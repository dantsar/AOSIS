#pragma once

void vmm_init(uint8_t *initial_page_table);

uint8_t *vmm_alloc_page(void);

void vmm_free_page(uint8_t *page);
