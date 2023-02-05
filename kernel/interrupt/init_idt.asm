; http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html#:~:text=The%20PIT%20(theory),18.2Hz%20and%201.1931%20MHz).
; http://www.osdever.net/bkerndev/Docs/idt.htm

extern idtp
extern isr_handler

global load_idt_asm
load_idt_asm:
    lidt [idtp]
    ret

global isr_common_stub
isr_common_stub:
    pushad               ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    push ds
    push es
    push fs
    push gs

    mov ax, GDT_DATA_SEG    ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler    ; extern isr_handler: can be found in interrupt.c

    pop ebx             ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popad               ; Pops edi,esi,ebp,esp,ebx,edx,ecx,eax

    pop gs
    pop fs
    pop es
    pop ds

    add esp, 8          ; Cleans up the pushed error code and pushed ISR number
    sti
    iret                ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


; the ISRs prepare the environment for the handler by saving the processor stat
; setting up kernel mode segments, and then calls the C-level fault handler
; the first push is a dummy error code for the corresponding exception
; the second push is the number of the isr.

; This macro creates a stub for an ISR which does NOT pass it's own
; error code (adds a dummy errcode byte).
; https://wiki.osdev.org/Interrupts_tutorial
%macro ISR_ERR 1
  global isr%1
  isr%1:
    cli                         ; Disable interrupts firstly.
    push byte 0                 ; Push a dummy error code.
    push byte %1                ; Push the interrupt number.
    jmp isr_common_stub         ; Go to our common handler code.
%endmacro

; This macro creates a stub for an ISR which passes it's own error code.
%macro ISR_REG 1
  global isr%1
  isr%1:
    cli                         ; Disable interrupts.
    push byte %1                ; Push the interrupt number
    jmp isr_common_stub
%endmacro

ISR_ERR 0
ISR_ERR 1
ISR_ERR 2
ISR_ERR 3
ISR_ERR 4
ISR_ERR 5
ISR_ERR 6
ISR_ERR 7
ISR_REG 8
ISR_ERR 9
ISR_REG 10
ISR_REG 11
ISR_REG 12
ISR_REG 13
ISR_REG 14
ISR_ERR 15
ISR_ERR 16
ISR_ERR 17
ISR_ERR 18
ISR_ERR 19
ISR_ERR 20
ISR_ERR 21
ISR_ERR 22
ISR_ERR 23
ISR_ERR 24
ISR_ERR 25
ISR_ERR 26
ISR_ERR 27
ISR_ERR 28
ISR_ERR 29
ISR_ERR 30
ISR_ERR 31

extern irq_handler
extern task_update_trapframe

; This is our common IRQ stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
irq_common_stub:
    pushad               ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    push ds

    mov ax, GDT_DATA_SEG    ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Save trapframe to the current task
    push esp
    call task_update_trapframe
    pop eax

    call irq_handler

    pop ds

    popad               ; Pops eax,ecx,edx...
    add esp, 8          ; Cleans up the pushed error code and pushed ISR number
    sti
    iret                ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
; This macro creates a stub for an IRQ - the first parameter is
; the IRQ number, the second is the ISR number it is remapped to.
%macro IRQ 2
global irq%1
irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47
