#pragma once

#include <stddef.h>
#include <stdint.h>

#include <memory/multiboot.h>

// Initialize page frame allocator
// accept mulitboot_info_t that is provided by GRUB
void pmm_init(struct multiboot_info *mbt);

// get page
uint8_t *pmm_get_page(void);

// free page
void pmm_free_page(uint8_t *page);
