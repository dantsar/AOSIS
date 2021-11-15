; http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html#:~:text=The%20PIT%20(theory),18.2Hz%20and%201.1931%20MHz).
; http://www.osdever.net/bkerndev/Docs/idt.htm

extern idtp
extern isr_handler

global _load_idt
_load_idt:
    lidt [idtp]
    ret


global isr_common_stub
isr_common_stub:
    pusha               ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds          ; Lower 16-bits of eax = ds.
    push eax            ; save the data segment descriptor

    ; mov ax, 0x10        ; load the kernel data segment descriptor
    ; mov ds, ax
    ; mov es, ax
    ; mov fs, ax
    ; mov gs, ax

    ; pusha 
    call isr_handler    ; extern isr_handler: can be found in interrupt.c

    pop ebx             ; reload the original data segment descriptor
    ; mov ds, bx
    ; jmp $
    ; mov es, bx
    ; mov fs, bx
    ; mov gs, bx

    popa                ; Pops edi,esi,ebp...
    add esp, 8          ; Cleans up the pushed error code and pushed ISR number
    sti
    iret                ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP 

global idt_iqr_remap:
idt_iqr_remap:
    
    ret


; the ISRs prepare the environment for the handler by saving the processor stat
; setting up kernel mode segments, and then calls the C-level fault handler
; the first push is a dummy error code for the corresponding exception 
; the second push is the number of the isr.
global isr0:
isr0:
    cli
    push byte 0 
    push byte 0
    jmp isr_common_stub
global isr1:
isr1:
    cli
    push byte 0 
    push byte 1
    jmp isr_common_stub
global isr2:
isr2:
    cli
    push byte 0 
    push byte 2
    jmp isr_common_stub
global isr3:
isr3:
    cli
    push byte 0 
    push byte 3
    jmp isr_common_stub
global isr4:
isr4:
    cli
    push byte 0 
    push byte 4
    jmp isr_common_stub
global isr5:
isr5:
    cli
    push byte 0 
    push byte 5
    jmp isr_common_stub
global isr6:
isr6:
    cli
    push byte 0 
    push byte 6
    jmp isr_common_stub
global isr7:
isr7:
    cli
    push byte 0 
    push byte 7
    jmp isr_common_stub
global isr8:
isr8:
    cli
    push byte 8
    jmp isr_common_stub
global isr9:
isr9:
    cli
    push byte 0 
    push byte 9
    jmp isr_common_stub
global isr10:
isr10:
    cli
    push byte 10
    jmp isr_common_stub
global isr11:
isr11:
    cli
    push byte 11
    jmp isr_common_stub
global isr12:
isr12:
    cli
    push byte 12
    jmp isr_common_stub
global isr13:
isr13:
    cli
    push byte 13
    jmp isr_common_stub
global isr14:
isr14:
    cli
    push byte 14
    jmp isr_common_stub
global isr15:
isr15:
    cli
    push byte 0 
    push byte 15
    jmp isr_common_stub
global isr16:
isr16:
    cli
    push byte 0 
    push byte 16
    jmp isr_common_stub
global isr17:
isr17:
    cli
    push byte 0 
    push byte 17
    jmp isr_common_stub
global isr18:
isr18:
    cli
    push byte 0 
    push byte 18
    jmp isr_common_stub

; the rest are reserved exceptions
global isr19:
isr19:
    cli
    push byte 0 
    push byte 19
    jmp isr_common_stub
global isr20:
isr20:
    cli
    push byte 0 
    push byte 20
    jmp isr_common_stub
global isr21:
isr21:
    cli
    push byte 0 
    push byte 21
    jmp isr_common_stub
global isr22:
isr22:
    cli
    push byte 0 
    push byte 22
    jmp isr_common_stub
global isr23:
isr23:
    cli
    push byte 0 
    push byte 23
    jmp isr_common_stub
global isr24:
isr24:
    cli
    push byte 0 
    push byte 24
    jmp isr_common_stub
global isr25:
isr25:
    cli
    push byte 0 
    push byte 25
    jmp isr_common_stub
global isr26:
isr26:
    cli
    push byte 0 
    push byte 26
    jmp isr_common_stub
global isr27:
isr27:
    cli
    push byte 0 
    push byte 27
    jmp isr_common_stub
global isr28:
isr28:
    cli
    push byte 0 
    push byte 28
    jmp isr_common_stub
global isr29:
isr29:
    cli
    push byte 0 
    push byte 29
    jmp isr_common_stub
global isr30:
isr30:
    cli
    push byte 0 
    push byte 30
    jmp isr_common_stub
global isr31:
isr31:
    cli
    push byte 0 
    push byte 31
    jmp isr_common_stub
