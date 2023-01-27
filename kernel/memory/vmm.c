#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <common/common_macros.h>
#include <kernel.h>
#include <memory/memory.h>
#include <memory/kmalloc.h>
#include <memory/paging.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <terminal/tty.h>

#define VMM_BITMAP_SIZE (((PAGE_TABLE_RANGE / PAGE_SIZE) / (sizeof(uint32_t) * 8)))

#define HIGH_WATER_THRESHOLD (1022) // Leave a buffer of two pages

struct vmm_block_desc
{
    uint32_t bitmap[VMM_BITMAP_SIZE]; // Each bit is a page (4KiB)

    size_t high_water_mark; // Index in the page table of the first unallocated page
    size_t free_pages;      // Number of available pages

    void *first_page; // The virtual address of the first page in the vmm block
    void *page_table; // Virtual address of the vmm block's page table

    struct vmm_block_desc *next_vmm_block;
};

struct vmm_block_desc *vmm_block_list_head = NULL;
struct vmm_block_desc *vmm_block_list_tail = NULL;

extern uint32_t kernel_base_addr_virt;

// Find a free page in the page table, return the index of the page in the page table
static bool vmm_get_page_from_bitmap(uint32_t *bitmap, uint32_t *index_in_bitmap);

// Create a new page table to expand the virtual address space
static void vmm_add_vmm_block(struct vmm_block_desc *vmm_block);

static bool vmm_get_page_from_bitmap(uint32_t *bitmap, uint32_t *index_in_bitmap)
{
    bool page_found = false;
    // Check if page is available in vmm block
    for (size_t i = 0; (i < VMM_BITMAP_SIZE) && !page_found; i++)
    {
        uint32_t page_bitmap = bitmap[i];
        if (page_bitmap == 0xFFFFFFFF)
        {
            // Skip because all the pages are allocated
            continue;
        }

        // Find and allocate the available page
        for (size_t j = 0; j < (sizeof(uint32_t) * 8) && !page_found; j++)
        {
            // find first bit that is not set
            if (!CHECK_BIT_SET(page_bitmap, j))
            {
                *index_in_bitmap = (32U * i) + j;

                // set bit in the bitmap
                bitmap[i] = SET_BIT(page_bitmap, j);
                page_found = true;
            }
        }
    }

    return page_found;
}

static void vmm_add_vmm_block(struct vmm_block_desc *vmm_block)
{
    struct vmm_block_desc *new_vmm_block = (struct vmm_block_desc *)kmalloc(sizeof(struct vmm_block_desc));
    memset(new_vmm_block, 0, sizeof(struct vmm_block_desc));

    // Initialize the new vmm block descriptor
    new_vmm_block->high_water_mark = 0;
    new_vmm_block->free_pages      = PAGE_TABLE_INDICES;
    new_vmm_block->next_vmm_block = NULL;

    // Virtual address of the next vmm block's page table
    new_vmm_block->page_table = (void *)((uint32_t)vmm_block->first_page + (HIGH_WATER_THRESHOLD * PAGE_SIZE));
    new_vmm_block->first_page = (void *)((uint32_t)vmm_block->first_page + PAGE_TABLE_RANGE);

    // Allocate the new page table
    void *phys_addr = paging_map_virtual_page(vmm_block->page_table, new_vmm_block->page_table);
    paging_add_page_table(phys_addr, new_vmm_block->first_page);

    memset(new_vmm_block->page_table, 0, PAGE_SIZE); // clear the new page table

    // Append new vmm block to the end of the list
    vmm_block_list_tail->next_vmm_block = new_vmm_block;
    vmm_block_list_tail = new_vmm_block;
}

void vmm_init(void *initial_page_table)
{
    struct vmm_block_desc *initial_vmm_block = (struct vmm_block_desc *)kmalloc(sizeof(struct vmm_block_desc));

    initial_vmm_block->first_page     = (void *)&kernel_base_addr_virt;
    initial_vmm_block->page_table     = initial_page_table;
    initial_vmm_block->next_vmm_block = NULL;

    // Reserve pages for the kernel image
    uint32_t first_free_page = ((uint32_t)V2P_ADDR(initial_page_table) + PAGE_SIZE) / PAGE_SIZE;
    initial_vmm_block->high_water_mark = first_free_page;
    initial_vmm_block->free_pages = (PAGE_TABLE_INDICES - first_free_page);

    // Set the bitmap
    uint32_t last_fully_occupied_bitmap_index = (first_free_page >> 5);
    memset(initial_vmm_block->bitmap, (int)0xFF, (last_fully_occupied_bitmap_index * 4));

    // set the last bitmap index
    uint32_t remaining_pages = (first_free_page % 32);
    uint32_t remaining_bits_to_set = ~(0xFFFFFFFF << remaining_pages);
    initial_vmm_block->bitmap[last_fully_occupied_bitmap_index] = remaining_bits_to_set;

    // Initialize vmm block list
    vmm_block_list_head = initial_vmm_block;
    vmm_block_list_tail = initial_vmm_block;
}

void *vmm_alloc_page(void)
{
    // loop throught the list and find an entry in the page range
    void *vmm_page = NULL;

    struct vmm_block_desc *vmm_block = vmm_block_list_head;
    while (vmm_block != NULL)
    {
        if (vmm_block->free_pages == 0U)
        {
            vmm_block = vmm_block->next_vmm_block;
            continue;
        }

        uint32_t index_in_bitmap = 0;
        bool page_found = vmm_get_page_from_bitmap(vmm_block->bitmap, &index_in_bitmap);

        if (page_found)
        {
            vmm_page = (void *)((uint32_t)vmm_block->first_page + (index_in_bitmap * PAGE_SIZE));

            vmm_block->free_pages--;

            uint32_t next_page = index_in_bitmap + 1;
            if (next_page >= vmm_block->high_water_mark)
            {
                vmm_block->high_water_mark = next_page;
            }

            break;
        }
        else
        {
            vmm_block = vmm_block->next_vmm_block;
        }
    }

    if (vmm_page == NULL)
    {
        panic("out of memory\n");
    }

    static bool allocating_new_block = false; // Kludge because kmalloc might allocate a page while createing a new vmm block

    if (vmm_block->high_water_mark == HIGH_WATER_THRESHOLD &&
        vmm_block->next_vmm_block == NULL &&
        !allocating_new_block)
    {
        allocating_new_block = true;
        vmm_add_vmm_block(vmm_block);
        allocating_new_block = false;
    }

    bool is_vmm_page_mapped = paging_is_virtual_page_present(vmm_block->page_table, vmm_page);

    if (!is_vmm_page_mapped)
    {
        paging_map_virtual_page(vmm_block->page_table, vmm_page);
    }
    else
    {
        // Virtual page is already mapped in memory
    }

    return vmm_page;
}

void vmm_free_page(void *page)
{
    uint32_t page_to_free = PAGE_ALIGN((uint32_t)page);

    struct vmm_block_desc *vmm_block = vmm_block_list_head;

    bool page_freed = false; // flag used for escaping the nested for loops
    while (vmm_block != NULL && !page_freed)
    {
        // In the list of vmm blocks, find the block that the page belongs to

        // Calculate the range of the vmm block
        const uint32_t page_table_base_page = (uint32_t)vmm_block->first_page;
        const uint32_t page_table_last_page  = (uint32_t)vmm_block->first_page + PAGE_TABLE_RANGE - PAGE_SIZE;

        if ((page_to_free >= page_table_base_page) || (page_to_free <= page_table_last_page))
        {
            // Page in vmm block range
            uint32_t page_index    = V2P_ADDR((uint32_t)page) / PAGE_SIZE;
            uint32_t bitmap_index  = page_index / 32U;
            uint32_t bit_in_bitmap = page_index % 32U;

            vmm_block->bitmap[bitmap_index] = CLEAR_BIT(vmm_block->bitmap[bitmap_index], bit_in_bitmap);

            page_freed = true;
        }
        else
        {
            // Page not in vmm block range
        }

        vmm_block = vmm_block->next_vmm_block;
    }

    if (!page_freed)
    {
        panic("invalid page provided\n");
    }
}
