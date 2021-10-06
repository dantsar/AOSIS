; print.inc : provide printing capabilities while in real mode

; ------------------------------------------------
; print out character using VGA
print_char:
    mov cl, al    ; save char being printed

    movzx ax, byte [ypos]   ; ax = ypos
    mov dx, 160             ; 160 b/c each pixel is rep. by two bytes (&& 80 lines)
    mul dx                  ; ax = ax * dx ; ax is byte pos

    movzx bx, byte [xpos]   ; bx = xpos
    shl bx, 1               ; mul by two 

    ; address of pixel in video mem
    mov di, 0   ; address video mem
    add di, ax  ; add ypos
    add di, bx  ; add xpos
    
    ; attribute (0: foreground, 1: text color)
    mov al, cl      ; al == char
    mov ah, 0x0f    ;  white text
    stosw           ; write char to vga memory

    add byte [xpos], 1  ; inc xpos b/c char was written

    ret

; print out string to VGA mem
print_str_char: 
    call print_char ; used b/c fall throuh to print_str
print_str: 
    lodsb
    cmp al, 0
    jnz print_str_char
    jmp done_print_str 
done_print_str:
    ; mov al, 'F'
    ; call print_char
    add byte [ypos], 1
    mov byte [xpos], 0
    ret


; BIOS printing (hopefully deprecated)
bprint:
    lodsb
    cmp al, 0       ; set flag for zero
    jz bprint_done  ; finish if NULL character

    mov ah, 0x0E
    int 0x10
    jmp bprint
bprint_done:
    ret

; -----------------------------------------------

xpos:   db 0
ypos:   db 10
