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
 
; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates room for a
; small stack by creating a symbol at the bottom of it, then allocating 16384
; bytes for it, and finally creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
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
	; machine. Interrupts are disabled. Paging is disabled. The processor
	; state is as defined in the multiboot standard. The kernel has full
	; control of the CPU.
 
	; To set up a stack, we set the esp register to point to the top of our
	mov esp, stack_top

	; set up global stuff here: load GDT, enable PAGING ETC
	; call _init
 
	; high level kernel ABI requires that the stack be 16 bit aligned
	extern kernel_main
	call kernel_main

	; just do nothing if kernel_main unexpectadly returns 

	cli
.hang:	hlt
	jmp .hang
.end:
