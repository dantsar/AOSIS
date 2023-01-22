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

#define HIGH_WATER_THRESHOLD (1022)     // leave a buffer of two pages
#define PAGE_RANGE           (1 << 22U) // addressable memory from a single page table


struct vmm_block_desc
{
    uint32_t bitmap[VMM_BITMAP_SIZE]; // each bit is a page (4KiB)

    size_t high_water_mark;           // the first unallocated index in page table (0-1023)
    size_t free_pages;                // number of available pages

    uint8_t *first_page; // the virtual address of the first page in the vmm block
    uint8_t *page_table; // page table

    struct vmm_block_desc *next_vmm_block;
};

struct vmm_block_desc *vmm_block_list_head = NULL;
struct vmm_block_desc *vmm_block_list_tail = NULL;

extern uint32_t kernel_base_addr_virt;

// this can use some cleanup
static uint32_t vmm_get_page_from_bitmap(uint32_t *bitmap, uint8_t *first_page, uint8_t **vmm_page)
{
    uint32_t index_in_bitmap = 0;

    bool page_found = false;
    // Check if page is available in vmm block
    for (size_t i = 0; (i < VMM_BITMAP_SIZE) && !page_found; i++)
    {
        uint32_t page_bitmap = bitmap[i];
        if (page_bitmap == 0xFFFFFFFF)
        {
            // skip because all the pages are allocated
            continue;
        }

        // Find and allocate the available page
        for (size_t j = 0; j < (sizeof(uint32_t) * 8) && !page_found; j++)
        {
            // find first bit that is not set
            if (!CHECK_BIT_SET(page_bitmap, j))
            {
                index_in_bitmap = (32U * i) + j;
                *vmm_page = (uint8_t *)((uint32_t)first_page + (PAGE_SIZE * index_in_bitmap));

                // set bit in the bitmap
                bitmap[i] = SET_BIT(page_bitmap, j);

                page_found = true;
            }
        }
    }

    return index_in_bitmap;
}

// TODO: clean this up
void vmm_init(uint8_t *initial_page_table)
{
    // I'm assuming that no other page was allocated after the initial page table
    // and that initial_page_table is 4KiB aligned

    struct vmm_block_desc *initial_vmm_block = (struct vmm_block_desc *)kmalloc(sizeof(struct vmm_block_desc));

    // TODO: think of a better name for this
    uint32_t first_free_virt_page = ((uint32_t)V2P_ADDR(initial_page_table) + PAGE_SIZE) / PAGE_SIZE;

    initial_vmm_block->first_page     = (uint8_t *)&kernel_base_addr_virt;
    initial_vmm_block->page_table     = initial_page_table;
    initial_vmm_block->next_vmm_block = NULL;

    initial_vmm_block->high_water_mark = first_free_virt_page;
    initial_vmm_block->free_pages = (1024 - first_free_virt_page); // TODO: hmmmmm, this is weird... double check this later

    // Set the bitmap
    uint32_t last_fully_occupied_bitmap_index = (first_free_virt_page >> 5);
    memset(initial_vmm_block->bitmap, (int)0xFF, (last_fully_occupied_bitmap_index * 4));

    // set the last bitmap index
    uint32_t remaining_pages = (first_free_virt_page % 32);
    uint32_t remaining_bits_to_set = ~(0xFFFFFFFF << remaining_pages);
    initial_vmm_block->bitmap[last_fully_occupied_bitmap_index] = remaining_bits_to_set;

    // Initialize vmm block list
    vmm_block_list_head = initial_vmm_block;
    vmm_block_list_tail = initial_vmm_block;
}

// TODO: add more comments
uint8_t *vmm_alloc_page(void)
{
    // loop throught the list and find an entry in the page range
    uint8_t *vmm_page = NULL;

    struct vmm_block_desc *vmm_block = vmm_block_list_head;
    while (vmm_block != NULL)
    {
        if (vmm_block->free_pages == 0U)
        {
            vmm_block = vmm_block->next_vmm_block;
            continue;
        }

        uint32_t index_in_bitmap = vmm_get_page_from_bitmap(vmm_block->bitmap, vmm_block->first_page, &vmm_page);

        if (vmm_page != NULL) // TODO: go over these if statements THE HIGH WATER MARK HERE IS BEING TREATED HERE AS THE LAST ALLOCATED PAGE BUT IT SHOULD BE THE FIRST UNALLOCATED PAGE TODO: FIIIIIIIIIIIIX
        {
            vmm_block->free_pages--;

            if (index_in_bitmap > vmm_block->high_water_mark)
            {
                vmm_block->high_water_mark = index_in_bitmap;
            }

            break;
        }
        else
        {
            vmm_block = vmm_block->next_vmm_block;
        }
    }

    // I'm not convinced that this check does anything. It maybe that the pmm will panic before this occurs
    if (vmm_page == NULL)
    {
        panic("out of memory\n");
    }

    // TODO: I need to figure out the interaction for a kmalloc while allocating a new vmm block with 1022 and 1023

    /* ************ THIS IS UNTESTED AND HAS BUGS ************ */
    /* TODO: PUT THIS IN A STATIC FUNCTION */
    // assume vmm page is an "available" page in the virtual address space (either free or needs to be allocated)
    static bool allocating_new_block = false;
    if (vmm_block->high_water_mark == HIGH_WATER_THRESHOLD && vmm_block->next_vmm_block == NULL && !allocating_new_block)
    {
        allocating_new_block = true; // potential infinite loop if kmalloc calls vmm_alloc_page, so add a flag for now

        // INITIALIZE THE VMM_BLOCK DESCRIPTOR
        // I'm allocating the last few pages for expanding the vmm block
        struct vmm_block_desc *new_vmm_block = (struct vmm_block_desc *)kmalloc(sizeof(struct vmm_block_desc));
        memset(new_vmm_block, 0U, sizeof(struct vmm_block_desc));

        new_vmm_block->free_pages      = PAGE_TABLE_INDICES;
        new_vmm_block->high_water_mark = 0U;

        // get the last page in the current vmm block and allocate it as a page table for the next vmm block
        uint32_t index_in_bitmap = HIGH_WATER_THRESHOLD;
        uint8_t *new_page_table = (uint8_t *)((uint32_t)vmm_block->first_page + (PAGE_SIZE * index_in_bitmap));
        memset(new_page_table, 0, PAGE_SIZE);

        new_vmm_block->page_table = new_page_table; // the sus imposter
        new_vmm_block->first_page = (uint8_t *)((uint32_t)vmm_block->first_page + PAGE_RANGE);

        // append new vmm block to the list of vmm blocks
        new_vmm_block->next_vmm_block = NULL;
        vmm_block->next_vmm_block     = new_vmm_block;

        // I'm not sure that this in the right place, however, I'll figure that out later

        // allocate a physical page for the new page table
        uint8_t *virtual_page = vmm_block->first_page + (PAGE_SIZE * HIGH_WATER_THRESHOLD);
        uint8_t *phys_page    = paging_populate_virtual_page(vmm_block->page_table, virtual_page);
        paging_add_page_table(phys_page, new_vmm_block->first_page);

        allocating_new_block = false;
    }

    // if vmm_page != NULL ==> vmm_block != NULL

    // TODO: Make this a static function
    bool is_vmm_page_mapped = paging_is_virtual_page_present(vmm_block->page_table, vmm_page);

    if (!is_vmm_page_mapped)
    {
        paging_populate_virtual_page(vmm_block->page_table, vmm_page);
    }
    else
    {
        // Virtual page is already mapped in memory
    }

    return vmm_page;
}

// TODO: add more comments
void vmm_free_page(uint8_t *page)
{
    uint32_t page_to_clear = PAGE_ALIGN((uint32_t)page);

    struct vmm_block_desc *vmm_block = vmm_block_list_head;

    bool page_freed = false;
    while (vmm_block != NULL && !page_freed)
    {
        // if page in page table range
        const uint32_t page_table_base_page = (uint32_t)vmm_block->first_page;
        const uint32_t page_table_last_page  = (uint32_t)vmm_block->first_page + PAGE_TABLE_RANGE - PAGE_SIZE;

        if ((page_to_clear >= page_table_base_page) || (page_to_clear <= page_table_last_page))
        {
            uint32_t page_index    = V2P_ADDR((uint32_t)page) / PAGE_SIZE;
            uint32_t bitmap_index  = page_index / 32U;
            uint32_t bit_in_bitmap = page_index % 32U;

            vmm_block->bitmap[bitmap_index] = CLEAR_BIT(vmm_block->bitmap[bitmap_index], bit_in_bitmap);

            page_freed = true;
        }
        else
        {
            // page not in vmm_block
        }

        vmm_block = vmm_block->next_vmm_block;
    }

    if (!page_freed)
    {
        panic("invalid page provided\n");
    }
}
