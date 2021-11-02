; http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html#:~:text=The%20PIT%20(theory),18.2Hz%20and%201.1931%20MHz).

extern idtp
global _load_idt
_load_idt:
    lidt [idtp]
    ret

; define struct in nasm for the IDT Descriptor
; create a 256 table of IDT Descriptors
; load to IDT into the memory 

; define struct in interrupt.h
; extern idt
; master slave PIC

; IRQ 0: timer
; IRQ 1: keyboard




; IDT_INFO:
;     dw IDT_END - IDT_START - 1
;     dd IDT_START

; IDT_START:



; IDT_END:
