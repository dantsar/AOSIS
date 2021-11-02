CODE_SEG equ GDT_code - GDT_START
; DATA_SEG equ GDT_data - GDT_start

; Reference resource
; https://web.archive.org/web/20190424213806/http://www.osdever.net/tutorials/view/the-world-of-protected-mode
__init_gdt__:
    cli
    lgdt [GDT_DESCRIPTOR] ; load the GDT
    
    ; change last bit of cr0 to one to actually 
    ; switch to the GDT, but can't change it directly
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; clear instruction pipeline by performing a far jump
    jmp CODE_SEG:clear_pipeline
[BITS 32]
clear_pipeline:
    ret

; https://en.wikipedia.org/wiki/Global_Descriptor_Table
GDT_START:
    GDT_null:
        dq 0x0  ; beginning of a GDT needs to have two null bytes

    ; code segment descriptor
    GDT_code: 
        dw 0xffff   ; segment limit (largest amount)
        dw 0x0      ; base first 0-15 bits
        db 0x0      ; base 16-23 bits
        db 0b10011010 ; access byte
        db 0b11001111 ; high 4 bits(flags) low 4 bites ()
        db 0x0      ; base 24-32 bits

    ; data segment descriptor
    GDT_data:
        dw 0xffff   ; segment limit (largest amount)
        dw 0x0      ; base first 0-15 bits
        db 0x0      ; base 16-23 bits
        db 0b10010010 ; access byte
        db 0b11001111 ; high 4 bits(flags) low 4 bites
        db 0x0      ; base 24-32 bits
GDT_END:    ; used to calculate size of GDT

; the GDT descriptor 
GDT_DESCRIPTOR:
    dw GDT_END - GDT_START - 1  ; size
    dd GDT_START                ; pointer the to start of the GDT
