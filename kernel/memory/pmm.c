#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <common/common_macros.h>
#include <kernel.h>
#include <memory/pmm.h>
#include <memory/multiboot.h>
#include <terminal/tty.h>

extern const uint8_t *kernel_end_addr_phys;
extern const uint32_t kernel_size;

struct physical_memory
{
    size_t total_memory;  // total system memory in bytes (including kernel and BIOS)

    size_t   total_page_memory;   // total memory of all the pages (4KiB)
    uint8_t  *page_starting_addr; // starting address of the first page

    size_t   page_count;              // total number of page frames
    uint32_t page_frame_bitmap[8192]; // bitmap of all the pages (set : allocated)
};

static struct physical_memory pmm = {
    .total_memory       = 0U,
    .total_page_memory  = 0U,
    .page_starting_addr = NULL,
    .page_count         = 0U,
    .page_frame_bitmap  = { 0 }
};

void pmm_init(struct multiboot_info *mbt)
{
	// check the sixth bit of the flags to see if grub provided a valid memory map
	if (!((mbt->flags >> 6) & 0x01)) {
		panic("invalid GRUB memory map");
	}

	uint8_t *last_mem_addr;
	for (size_t i = 0; i < mbt->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t *entry = (multiboot_memory_map_t *) (mbt->mmap_addr + i);

        // only use available memory regions
		if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
			pmm.total_memory = entry->len_low;

			// last memory address
			last_mem_addr = (uint8_t *) (entry->addr_low + entry->len_low);
		}
        else
        {
            // skip memory regions
        }
	}

    // populate physical_memory struct here with information

    // addr of the first page
    uint32_t first_page_addr = PAGE_ALIGN((uint32_t)&kernel_end_addr_phys);
    if (first_page_addr < (uint32_t)&kernel_end_addr_phys)
    {
        first_page_addr = PAGE_INCREMENT(first_page_addr);
    }

    pmm.page_starting_addr = (uint8_t *)first_page_addr;

    // trim the last page to make sure that it is entirely 4KiB
    uint32_t last_page_addr = PAGE_ALIGN((uint32_t)last_mem_addr);
    if (PAGE_INCREMENT(last_page_addr) > (uint32_t)last_mem_addr)
    {
        last_page_addr = PAGE_DECREMENT(last_page_addr);
    }

    pmm.total_page_memory = PAGE_INCREMENT(last_page_addr) - first_page_addr;
    pmm.page_count        = (pmm.total_page_memory / PAGE_SIZE);
}

// find and allocate a page from the page pool,
// return the physical address of the page
uint8_t *pmm_alloc_page(void)
{
    // Physical address of allocated page
    uint8_t *alloc_page_addr = NULL;

    bool page_found = false; // flag used for escaping for loops
    for (uint32_t i = 0; i < (pmm.page_count / 32U) && !page_found; i++)
    {
        uint32_t page_bitmap = pmm.page_frame_bitmap[i];

        if (page_bitmap == 0xFFFFFFFFU)
        {
            // skip because all pages are allocated
            continue;
        }

        // find first unset bit to allocate page
        for (uint32_t j = 0; j < 32U && !page_found; j++)
        {
            // find first bit that is not set
            if (!CHECK_BIT_SET(page_bitmap, j))
            {
                // get physical address of the allocated page
                alloc_page_addr = (uint8_t *)((uint32_t)pmm.page_starting_addr + (PAGE_SIZE * (32U * i)) + (PAGE_SIZE * j));

                // set bit in the bitmap
                pmm.page_frame_bitmap[i] = SET_BIT(page_bitmap, j);
                page_found = true;
            }
        }
    }

    if (alloc_page_addr == NULL)
    {
        panic("all pages are allocated\n");
    }

    return alloc_page_addr;
}

// free page by clearing corresponding bit in the bitmap
void pmm_free_page(uint8_t *page)
{
    uint32_t page_to_clear = PAGE_ALIGN((uint32_t)page);

    // check if page is in page pool
    const uint32_t last_page_addr = (uint32_t)pmm.page_starting_addr * (PAGE_SIZE * (pmm.page_count  - 1U));
    if ((page_to_clear >= (uint32_t)pmm.page_starting_addr) &&
        (page_to_clear <= last_page_addr))
    {
        // get page index within page pool bitmap
        const uint32_t page_index    = (page_to_clear - (uint32_t)pmm.page_starting_addr) / PAGE_SIZE;
        const uint32_t bitmap_index  = page_index / 32U;
        const uint32_t bit_in_bitmap = page_index % 32U;

        pmm.page_frame_bitmap[bitmap_index] = CLEAR_BIT(pmm.page_frame_bitmap[bitmap_index], bit_in_bitmap);
    }
    else
    {
        panic("page not in page pool");
    }
}
