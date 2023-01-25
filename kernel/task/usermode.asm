

section .multiboot.text
task_userspace_asm:
    jmp task_userspace_asm


extern task_userspace
extern user_stack

section .text
global switch_to_usermode_asm
switch_to_usermode_asm:
    cli

    ; use the user data descriptor
    mov eax, 0x2b
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax

    ; push the user data descriptor
    push eax    ; SS

    ; push the user stack
    mov eax, user_stack
    push eax    ; ESP

    pushfd  ; eflags
    pop eax
    or eax, 0x200
    push eax

    push 0x23 ; CS
    push task_userspace ; EIP

    iretd
