#pragma once

void vmm_init(void *initial_page_table);

void *vmm_alloc_page(void);

void vmm_free_page(void *page);

uint32_t vmm_get_phys_addr(void *vmm_page);
