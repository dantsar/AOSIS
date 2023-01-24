#pragma once

#include <stddef.h>
#include <stdint.h>

#include <memory/memory.h>
#include <memory/multiboot.h>

#define PAGE_SIZE (4096U) // size of page in bytes

#define PAGE_ALIGN(x)     ((x) & 0xFFFFF000U)
#define PAGE_INCREMENT(x) ((x) + PAGE_SIZE)
#define PAGE_DECREMENT(x) ((x) - PAGE_SIZE)

// Initialize page frame allocator
// accept mulitboot_info_t that is provided by GRUB
void pmm_init(struct multiboot_info *mbt);

// allocate a new page (find the first one available)
// return the physical address of the page
void *pmm_alloc_page(void);

// free page
void pmm_free_page(void *page);
