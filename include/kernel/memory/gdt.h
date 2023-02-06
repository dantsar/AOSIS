#pragma once

#include <stdint.h>

#define GDT_KERNEL_CODE_SEG (0x08U)
#define GDT_KERNEL_DATA_SEG (0x10U)
#define GDT_TSS_SEG         (0x18U)

#define GDT_USER_CODE_SEG   (0x23U)
#define GDT_USER_DATA_SEG   (0x2BU)

uint32_t gdt_init();

void gdt_update_tss(uint32_t user_kernel_stack);
