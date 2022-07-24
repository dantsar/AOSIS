#ifndef _PMM_H
#define _PMM_H

#include <memory/multiboot.h>
#include <common/bitmap.h>

struct memory_range {
    size_t mem_start;
    size_t len;
};
typedef struct memory_range memrange_t;

struct pmm {
    uint64_t total_mem;
    bitmap_t bmap;
    memrange_t *mmap;
    size_t mem_ranges;
};
typedef struct pmm pmm_t;


// Initialize page frame allocator
// accept mulitboot_info_t that is provided by GRUB
void pmm_init(multiboot_info_t *mbt);

#endif
