GDT_KERNEL_CODE_SEG: equ (0x08)
GDT_KERNEL_DATA_SEG: equ (0x10)
GDT_TSS_SEG: equ (0x18)

section .text

global gdt_init_asm
gdt_init_asm:
    cli

    ; call the C function that setups up the all the descriptors in the GDT
    ; especially the TSS
    extern gdt_init
    call gdt_init
    lgdt [eax] ; load the GDT

    ; set first bit of cr0 to actually switch the GDT
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; set CS by performing a far jump
    jmp GDT_KERNEL_CODE_SEG:.finish_gdt_init

    [BITS 32]
    .finish_gdt_init:

        ; load the data segment into all the segment registers
        mov ax, GDT_KERNEL_DATA_SEG
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        ret

global gdt_load_tss_asm
gdt_load_tss_asm:
    mov ax, GDT_TSS_SEG
    ltr ax
    ret
