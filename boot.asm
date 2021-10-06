; boot.asm 

; === SHI-TUFF TO DO === 
;; VGA printing
;   -> text wraparound 
;   -> line wraparound
; IRT - Interrupt table
; GDT - Global descriptor table

; -----------------------------------------------
[BITS 16]
    jmp start
    %include "print.inc"


start:
    ; manually set up the segments : used for calculating addresses
    ; trust no one... so don't trust ORG
; [ORG 0x7C00]
    mov ax, 0x07c0 
    mov ds, ax
    mov ss, ax
    mov ax, 0xb800
    mov es, ax

    mov sp, 0x9c00  ; set up stack 
    
    cld     ; clear direction for reading string

    mov si, msg1
    call print_str 
    mov si, msg2
    call print_str

    ; printing using bios 
    ; mov si, msg1
    ; call bprint
    ; mov si, msg2
    ; call bprint

    jmp $

; -----------------------------------------------

; -----------------------------------------------
msg1:   db 'WELCOME TO AOSIS!', 0 ; , 0xD, 0xA, 0     ; carr ret, newline, NULL
msg2:   db 'THE POORLY MADE OS', 0

; pad sector with zeros and add magic byte 0x55aa to end of boot sector
times 510-($-$$) db 0
dw 0xaa55
; db 0x55
; db 0xaa
