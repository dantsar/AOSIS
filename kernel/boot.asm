; this boot.asm was based on https://wiki.osdev.org/Bare_Bones

; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot
 
; define .multiboot section as required by the linker 
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
 
%include "init_gdt.inc"
%include "init_idt.inc"

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:
; The linker script specifies _start as the entry point to the kernel and the
section .text
global _start:function (_start.end - _start)
_start:
	; The bootloader has loaded us into 32-bit protected mode on a x86
	; machine. Interrupts are disabled. Paging is disabled.
 
	; To set up a stack, we set the esp register to point to the top of our
	mov esp, stack_top

	; initialize all global constructs that are needed by the operaing system 
	; first set up the GDT followed by the IDT
	call __init_gdt__
	; call __init_idt__
 
	; high level kernel ABI requires that the stack be 16 bit aligned
	extern kmain
	call kmain

	; just do nothing if kernel_main unexpectadly returns 

	cli
.hang:	hlt
	jmp .hang
.end:
