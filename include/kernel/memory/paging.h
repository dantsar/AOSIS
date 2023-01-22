#pragma once

#include <stdbool.h>
#include <stdint.h>

#define PAGE_TABLE_INDICES (1024U)
#define PAGE_TABLE_RANGE   (PAGE_TABLE_INDICES * PAGE_SIZE)

uint8_t *paging_init();

void paging_add_page_table(uint8_t *pt_phys_addr, uint8_t *pt_virt_addr);

bool paging_is_virtual_page_present(uint8_t *page_table_ptr, uint8_t *virt_page);

// Returns the physcial address of the virtual page
uint8_t *paging_populate_virtual_page(uint8_t *page_table_ptr, uint8_t *virt_page);

// just copy over the page directory entries
void paging_copy_virtual_address_space(uint8_t *src, uint8_t * dest);
