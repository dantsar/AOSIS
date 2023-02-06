section .text

extern current_task

;C declaration:
;   void task_switch_asm(struct task *next_task);
global task_switch_asm
task_switch_asm:
    cli ; disable interrupts

    mov esi, [esp + 4]      ; arg1: current_task->next_task
    mov [current_task], esi ; Switch to the next task

    ; Restore trapframe
    mov esp, [esi + TASK_TRAPFRAME]

    ; Restore the context
    pop ds
    popad

    ; get rid of interrupt number and error code
    add esp, 8

    ; Set eflags to enable interrupts
    or dword [esp + 8], 0x200

    iret

TASK_TRAPFRAME:  equ 4
