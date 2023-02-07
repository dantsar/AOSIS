#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <common/common_macros.h>
#include <kernel.h>
#include <memory/pmm.h>
#include <memory/multiboot.h>
#include <task/task.h>
#include <terminal/tty.h>

extern const uint8_t *kernel_end_addr_phys;
extern const uint32_t kernel_size;

struct physical_memory
{
    size_t total_memory;  // total system memory in bytes (including kernel and BIOS)

    size_t   total_page_memory;   // total memory of all the pages (4KiB)
    uint8_t  *starting_addr; // starting address of the first page

    size_t   page_count;              // total number of page frames
    uint32_t bitmap[8192]; // bitmap of all the pages (set : allocated)
};

static struct physical_memory pmm = {
    .total_memory       = 0U,
    .total_page_memory  = 0U,
    .starting_addr = NULL,
    .page_count         = 0U,
    .bitmap  = { 0 }
};

static void pmm_init_struct(void *last_mem_addr)
{
    // addr of the first page
    uint32_t first_page_addr = PAGE_ALIGN((uint32_t)&kernel_end_addr_phys);
    if (first_page_addr < (uint32_t)&kernel_end_addr_phys)
    {
        first_page_addr = PAGE_INCREMENT(first_page_addr);
    }

    pmm.starting_addr = (uint8_t *)first_page_addr;

    // trim the last page to make sure that it is entirely 4KiB
    uint32_t last_page_addr = PAGE_ALIGN((uint32_t)last_mem_addr);
    if (PAGE_INCREMENT(last_page_addr) > (uint32_t)last_mem_addr)
    {
        last_page_addr = PAGE_DECREMENT(last_page_addr);
    }

    pmm.total_page_memory = PAGE_INCREMENT(last_page_addr) - first_page_addr;
    pmm.page_count        = (pmm.total_page_memory / PAGE_SIZE);
}

static void pmm_init_user_task(uint32_t mods_count, void *mod_list)
{
    // struct multiboot_mod_list *mod_list = (struct multiboot_mod_list *)mbt->mods_addr;

    UNUSED(mods_count);

    struct multiboot_mod_list *user_module = (struct multiboot_mod_list *)(mod_list + 0 * sizeof(struct multiboot_mod_list));

    // I need to reserve the memory (assuming that the size of the user task is <PAGE_SIZE)
    uint32_t page = (user_module->mod_start - (uint32_t)pmm.starting_addr);

    uint32_t bitmap_index = (page >> 5);
    uint32_t bitmap_bit = ~(0xFFFFFFFF << (page % 32));

    pmm.bitmap[bitmap_index] = SET_BIT(pmm.bitmap[bitmap_index], bitmap_bit);

    // I need to save the starting address
    struct user_task_info user_task = {
        .starting_eip = 0U,
        .phys_addr_start = user_module->mod_start,
        .phys_addr_end = user_module->mod_end,
        .len = (user_module->mod_end - user_module->mod_start)
    };

    task_init_user_task_info(user_task);
}

void pmm_init(struct multiboot_info *mbt)
{
    // check the sixth bit of the flags to see if grub provided a valid memory map
    if (!((mbt->flags >> 6) & 0x01)) {
        panic("invalid GRUB memory map");
    }

    void *last_mem_addr;
    for (size_t i = 0; i < mbt->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t *entry = (multiboot_memory_map_t *) (mbt->mmap_addr + i);

        // only use available memory regions
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            pmm.total_memory = entry->len_low;

            // last memory address
            last_mem_addr = (void *) (entry->addr_low + entry->len_low);
        }
        else
        {
            // skip memory regions
        }
    }

    pmm_init_struct(last_mem_addr);

    // init template user task
    pmm_init_user_task(mbt->mods_count,(void *) mbt->mods_addr);
}

// find and allocate a page from the page pool,
// return the physical address of the page
void *pmm_alloc_page(void)
{
    // Physical address of allocated page
    void *alloc_page_addr = NULL;

    bool page_found = false; // flag used for escaping for loops
    for (uint32_t i = 0; i < (pmm.page_count / 32U) && !page_found; i++)
    {
        uint32_t page_bitmap = pmm.bitmap[i];

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
                alloc_page_addr = (void *)((uint32_t)pmm.starting_addr + (PAGE_SIZE * (32U * i)) + (PAGE_SIZE * j));

                // set bit in the bitmap
                pmm.bitmap[i] = SET_BIT(page_bitmap, j);
                page_found = true;
            }
        }
    }

    if (alloc_page_addr == NULL)
    {
        kprintf("WARNING!!! All Physical pages are allocated\n");
    }

    return alloc_page_addr;
}

// free page by clearing corresponding bit in the bitmap
void pmm_free_page(void *page)
{
    uint32_t page_to_clear = PAGE_ALIGN((uint32_t)page);

    // check if page is in page pool
    const uint32_t last_page_addr = (uint32_t)pmm.starting_addr * (PAGE_SIZE * (pmm.page_count  - 1U));
    if ((page_to_clear >= (uint32_t)pmm.starting_addr) &&
        (page_to_clear <= last_page_addr))
    {
        // get page index within page pool bitmap
        const uint32_t page_index    = (page_to_clear - (uint32_t)pmm.starting_addr) / PAGE_SIZE;
        const uint32_t bitmap_index  = page_index / 32U;
        const uint32_t bit_in_bitmap = page_index % 32U;

        pmm.bitmap[bitmap_index] = CLEAR_BIT(pmm.bitmap[bitmap_index], bit_in_bitmap);
    }
    else
    {
        panic("page not in page pool");
    }
}
