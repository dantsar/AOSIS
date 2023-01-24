#pragma once

void vmm_init(uint8_t *initial_page_table);

void *vmm_alloc_page(void);

void vmm_free_page(void *page);
