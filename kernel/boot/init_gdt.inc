; Reference resource
; https://web.archive.org/web/20190424213806/http://www.osdever.net/tutorials/view/the-world-of-protected-mode
global __init_gdt__
__init_gdt__:
    cli
    lgdt [GDT_DESCRIPTOR] ; load the GDT

    ; set last bit of cr0 to actually switch to the GDT
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; set CS by performing a far jump
    jmp GDT_CODE_SEG:.finish_gdt_init

    [BITS 32]
    .finish_gdt_init:

        ; load the data segment into all the segment registers
        mov ax, GDT_DATA_SEG
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        ret

; https://en.wikipedia.org/wiki/Global_Descriptor_Table
GDT_START:
    GDT_null:
        dq 0x0  ; beginning of a GDT needs to have two null bytes

    ; code segment descriptor
    GDT_code:
        dw 0xffff       ; segment limit (largest amount)
        dw 0x0          ; base first 0-15 bits
        db 0x0          ; base 16-23 bits
        db 0b10011010   ; access byte (executable)
        db 0b11001111   ; high 4 bits(flags) low 4 bits ()
        db 0x0          ; base 24-32 bits

    ; data segment descriptor
    GDT_data:
        dw 0xffff       ; segment limit (largest amount)
        dw 0x0          ; base first 0-15 bits
        db 0x0          ; base 16-23 bits
        db 0b10010010   ; access byte (not executable)
        db 0b11001111   ; high 4 bits(flags) low 4 bits
        db 0x0          ; base 24-32 bits

GDT_END:   ; used to calculate size of GDT

; the GDT descriptor
GDT_DESCRIPTOR:
    dw GDT_END - GDT_START - 1  ; size
    dd GDT_START                ; pointer the to start of the GDT

GDT_DATA_SEG: equ (GDT_data - GDT_START)
GDT_CODE_SEG: equ (GDT_code - GDT_START)
