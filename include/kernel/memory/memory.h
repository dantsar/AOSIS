#pragma once

#include <stdint.h>

extern uint32_t kernel_base_addr_virt;

#define KERNEL_PHYS_BASE      (0x100000U)
#define KERNEL_VIRT_BASE ((uint32_t)&kernel_base_addr_virt)

#define P2V_ADDR(x) ((uint32_t)(x) + KERNEL_VIRT_BASE)
#define V2P_ADDR(x) ((uint32_t)(x) - KERNEL_VIRT_BASE)
