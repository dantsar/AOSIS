section .text

extern current_task
; extern task_test

; C declaration:
;   uint32_t task_switch_init_stack_asm(uint32_t kernel_stack_base, uint32_t eip);
global task_switch_init_stack_asm
task_switch_init_stack_asm:
    ; this is required to initialize the stack
    ; dummy instruction pointer
    mov eax, [esp + 4] ; arg1: stack to initialize

    ; Push the address of the task's first address
    mov ebx, [esp + 8] ; arg2: task eip
    mov [eax - 4], ebx ; eip

    sub eax, 20 ; 4 (bytes) * (5 dummy values)
    ret

;C declaration:
;   void task_switch_asm(struct task *next_thread);
global task_switch_asm
task_switch_asm:
    cli ; disable interrupts

    ; Save previous task's state:
    ;   Because of C Calling conventions, EAX, ECX, EDX are saved by the caller.
    ;   Moreover, EIP is saved by CALL
    push ebx
    push esi
    push edi
    push ebp

    ; save stack pointer to the current task struct
    mov edi, [current_task]
    mov [edi + TASK_KERNEL_ESP], esp ; Save the current task's esp in the task struct

    mov esi, [esp + 20]     ; arg1: current_task->next_task
    mov [current_task], esi ; Switch to the next task

    ; Load next task's state
    mov esp, [esi + TASK_KERNEL_ESP]      ;Load ESP for next task's kernel stack from the thread's TCB

    ; Restore previous task's state:
    pop ebp
    pop edi
    pop esi
    pop ebx

    sti     ; enable interrupts
    ret     ;Load next task's EIP from its kernel stack

TASK_KERNEL_ESP: equ 0 ; offset 0
TASK_STACK_TOP:  equ 4 ; offset sizeof(kernel_stack_top)
