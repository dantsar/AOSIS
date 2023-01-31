#pragma once

#include <stdint.h>

#define GDT_CODE_SEG (0x08U)
#define GDT_DATA_SEG (0x10U)
#define GDT_TSS_SEG  (0x18U)

uint32_t gdt_init();

void gdt_update_tss(uint32_t user_kernel_stack);
