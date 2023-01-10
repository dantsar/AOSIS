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

    kernel_virtual_base_addr = 0xC0000000;
    . += kernel_virtual_base_addr;
    kernel_start_addr_virt = .;

    .text ALIGN(4K) : AT(ADDR(.text) - kernel_virtual_base_addr)
    {
        *(.text)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - kernel_virtual_base_addr)
    {
        *(.rodata)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - kernel_virtual_base_addr)
    {
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - kernel_virtual_base_addr)
    {
        *(COMMON)
        *(.bss)

        kernel_end_addr_virt = .;
        kernel_end_addr_phys = . - kernel_virtual_base_addr;
        kernel_size = (kernel_end_addr_phys - kernel_start_addr_phys);
    }
}
