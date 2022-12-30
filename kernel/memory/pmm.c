#include <kernel.h>
#include <memory/pmm.h>
#include <memory/multiboot.h>
#include <terminal/tty.h>

#include <stdint.h>
#include <stddef.h>

#define PAGE_ALIGN(x)     ((x) & 0xFFFFF000U)
#define PAGE_INCREMENT(x) ((x) + 0x1000U)
#define PAGE_DECREMENT(x) ((x) - 0x1000U)

extern const uint8_t *kernel_end_address;
extern const uint32_t kernel_size;

struct physical_memory
{
    size_t total_memory;  // total system memory in bytes (including kernel and BIOS)

    size_t   total_page_memory;       // total memory of all the pages (4KiB)
    uint8_t  *page_starting_addr;     // starting address of the first page

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

	// debugging info, multiboot_info struct
	kprintf("MMAP_ADDR: 0x%x\n", mbt->mmap_addr);
	kprintf("MMAP_LENGTH: %d\n", mbt->mmap_length);
	kprintf("mmm_t size: %d\n", sizeof(multiboot_memory_map_t));

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

            // debugging info, memmory region
            kprintf("\tsize: %d\n"
                    "\taddr: 0x%x%x\n"
                    "\tlen: 0x%x%x\n",
                    entry->size,
                    entry->addr_high, entry->addr_low,
                    entry->len_high, entry->len_low);
		}
        else
        {
            kprintf("skipped\n");
        }
	}

    // populate physical_memory struct here with information
    uint32_t first_page_addr = PAGE_ALIGN((uint32_t)&kernel_end_address);
    if (first_page_addr < (uint32_t)&kernel_end_address)
    {
        first_page_addr = PAGE_INCREMENT(first_page_addr);
    }

    pmm.page_starting_addr = (uint8_t *)first_page_addr;

    uint32_t last_page_addr = PAGE_ALIGN((uint32_t)last_mem_addr);
    if (PAGE_INCREMENT(last_page_addr) > (uint32_t)last_mem_addr)
    {
        last_page_addr = PAGE_DECREMENT(last_page_addr);
    }

    pmm.total_page_memory = PAGE_INCREMENT(last_page_addr) - first_page_addr;
    pmm.page_count        = (pmm.total_page_memory / 4096U);

    // debugging info, physical memory struct
    kprintf("\tpmm.page_starting_addr: %x\n"
            "\tpmm.total_page_memory: %x\n"
            "\tpmm.total_memory: %x\n"
            "\tpmm.page_count: %x\n",
            (uint32_t)pmm.page_starting_addr,
            (uint32_t)pmm.total_page_memory,
            (uint32_t)pmm.total_memory,
            (uint32_t)pmm.page_count);

    kprintf("\nkernel_size: %x\n", &kernel_size);
}

uint8_t *pmm_get_page(void)
{
    uint8_t *page = NULL;
    for (uint32_t i = 0; i < (pmm.page_count / 32U); i++)
    {
        if (pmm.page_frame_bitmap[i] != 0xFFFFFFFFU)
        {
            // allocate page here
            for (uint32_t j = 0; j < 32U; j++)
            {

            }
            // page = /* blah */;
        }
        else
        {
            // skip because all pages are allocated
        }
    }

    if (page == NULL)
    {
        panic("all pages are allocated\n");
    }

    return page;
}

void pmm_free_page(uint8_t *page)
{
    // free page
}
