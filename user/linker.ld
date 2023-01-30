OUTPUT_FORMAT("binary")    /* output flat binary */

SECTIONS
{
    . = 0;

    .text ALIGN(4K):
    {
        start.o(.text)     /* include the .text section of start.o */
        *(.text)           /* include all other .text sections */
    }

    .data ALIGN(4K):
    {
        *(.data)
    }

    .rodata ALIGN(4K):
    {
        *(.rodata)
    }
}
