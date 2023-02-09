ENTRY(_start)

SECTIONS
{
    /* kernel is loaded at physical address 0x100000 */
    . = 1M;

    .multiboot.data :
    {
        *(.multiboot.data)
    }

    .multiboot.text :
    {
        *(.multiboot.text)
    }

    kernel_start_addr_phys = .;

    kernel_base_addr_virt = 0xC0000000;
    . += kernel_base_addr_virt;
    kernel_start_addr_virt = .;

    .text ALIGN(4K) : AT(ADDR(.text) - kernel_base_addr_virt)
    {
        *(.text)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - kernel_base_addr_virt)
    {
        *(.rodata)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - kernel_base_addr_virt)
    {
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - kernel_base_addr_virt)
    {
        *(COMMON)
        *(.bss)
    }

    kernel_end_addr_virt = .;
    kernel_end_addr_phys = . - kernel_base_addr_virt;
    kernel_size = (kernel_end_addr_phys - kernel_start_addr_phys);
}
