ENTRY(_start)

SECTIONS
{
	. = 1M;

	kernel_start_address = .;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
		kernel_end_address = .;
        kernel_size = (kernel_end_address - kernel_start_address);
	}

}
