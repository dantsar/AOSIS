#pragma once

#define KERNEL_PHYS_ADDR      (0x100000U)
#define KERNEL_VIRT_BASE_ADDR (0xC0000000U)

#define P2V_ADDR(x) ((x) + KERNEL_VIRT_BASE_ADDR)
#define V2P_ADDR(x) ((x) - KERNEL_VIRT_BASE_ADDR)
