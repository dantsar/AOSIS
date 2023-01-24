; this boot.asm was based on https://wiki.osdev.org/Bare_Bones

; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

; define .multiboot section as required by the linker
section .multiboot.data
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
; allocate stack
global stack_top

align 16
stack_bottom:
resb 4096 ; 4 KiB
stack_top:

; allocate temporary page directory and page table to enable the higher half kernel
align 4096
temp_page_directory:
    resb 4096
temp_page_table:
    resb 4096

; The linker script specifies _start as the entry point to the kernel and the
section .multiboot.text
global _start:function (_start.end - _start)    ; https://www.nasm.us/xdoc/2.11.08/html/nasmdoc6.html (6.6 GLOBAL)
_start:
	; The bootloader has loaded us into 32-bit protected mode on a x86
	; machine. Interrupts are disabled. Paging is disabled.

    ; To set up a stack, we set the esp register to point to the top of our
	mov esp, (stack_top - KERNEL_VIRTUAL_BASE)

	; arugments for kmain provided by grub
	; multiboot magic number
	push eax
	; address to struct multiboot_info
	push ebx

    ; add pages to the page table (assumes at least 4MB1of memory)
    mov eax, 0x0    ; starting page frame
    mov ebx, 0x0    ; page table entry
    mov ecx, 0      ; counter for loop
    mov edx, PAGES_IN_PAGE_TABLE
    .identity_map_kernel:
        ; from address 0x0 add each page to the page table
        mov ebx, eax
        or ebx, (PAGE_PRESENT | PAGE_WRITEABLE)

        ; write ebx to page table
        mov [(temp_page_table - KERNEL_VIRTUAL_BASE) + ecx * 4], ebx

        add eax, 0x1000 ; increment by a page

        inc ecx
        cmp ecx, edx
        jl .identity_map_kernel


    ; add page table to the page directory
    mov eax, (temp_page_table - KERNEL_VIRTUAL_BASE)
    or eax, (PAGE_PRESENT | PAGE_WRITEABLE)

    ; identity map the kernel and add the virtual address
    mov dword [(temp_page_directory - KERNEL_VIRTUAL_BASE) + 0x000 * 4], eax ; base address 0x00000000
    mov dword [(temp_page_directory - KERNEL_VIRTUAL_BASE) + 0x300 * 4], eax ; base address 0xC0000000

    ; load page directory to cr3
    mov eax, (temp_page_directory - KERNEL_VIRTUAL_BASE)
    mov cr3, eax

    ; enable paging by setting bit 31 in cr0
    mov eax, cr0
    or eax, 0x80000000 ; set bit 31
    mov cr0, eax       ; acutally enable paging here

    ; absolute jump to the higher half kernel
    lea ecx, [higher_half_kernel]
    jmp ecx
.end:

section .text

[BITS 32]
%include "interrupt/init_idt.asm"
%include "memory/init_gdt.asm"

higher_half_kernel:
    ; update stack to virtual address
    add esp, KERNEL_VIRTUAL_BASE

	; set up the GDT and initalize three entries
	; (NULL descriptor, data and code segment)
	; does not touch kmain args
	call gdt_init_asm

	; high level kernel ABI requires that the stack be 16 bit aligned
	extern kmain
	call kmain

	; just do nothing if kernel_main unexpectadly returns
	cli
    hlt

.hang:
	jmp .hang

; nasm MACROS (not actual date)
PAGE_WRITEABLE:      equ (0x01)
PAGE_PRESENT:        equ (0x02)
PAGES_IN_PAGE_TABLE: equ (1024)
KERNEL_VIRTUAL_BASE: equ (0xC0000000)
