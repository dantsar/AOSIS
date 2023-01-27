#pragma once

#include <stdbool.h>
#include <stdint.h>

#define PAGE_TABLE_INDICES (1024U)
#define PAGE_TABLE_RANGE   (PAGE_TABLE_INDICES * PAGE_SIZE)

void *paging_init();

// Insert the page table into the page directory
void paging_add_page_table(void *pt_phys_addr, void *pt_virt_addr);

bool paging_is_virtual_page_present(void *page_table_ptr, void *virt_page);

// Map a virtual page to a physical page and add to page table
// Returns the physcial address
void *paging_map_virtual_page(void *page_table_ptr, void *virt_page);

void *paging_map_virtual_page_user(void *page_table_ptr, void *virt_page);

// // just copy over the page directory entries
// void paging_copy_virtual_address_space(uint8_t *src, uint8_t * dest);
