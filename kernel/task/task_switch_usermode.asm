section .multiboot.text
task_userspace_asm:
    mov eax, 0x12121212
    jmp task_userspace_asm


section .text
extern user_stack
; extern task_users pace_asm
; C declaration:
; @Return stack_top
;   uint32_t task_switch_init_user_stack_asm(uint32_t user_stack);
; global task_switch_init_user_stack_asm
; task_switch_init_user_stack_asm:
;     ; init the user stack
; ebp + 4 ; arg1 user stack
;     mov eax, [ebp + 4]

;     mov [eax - 4], 0x2b ; Stack Segment
;     mov [eax - 8],

;     push 0x2b
;     push user_stack
;     push eflags with the shit
;     push cs (0x23)
;     push eip


; TODO: THIS NEEDS TO BE UPDATED, I SHOULD JUST PASS THE TRAPFRAME ON THE KERNELE STACK
; TODO: THIS SHOULD ALSO UPDATE THE CURRENT TASK TO THE USER TASK (go down the circular linked list)
; C declaration:
;   void task_switch_to_usermode_asm(uint32_t user_stack);
global task_switch_to_usermode_asm
task_switch_to_usermode_asm:
    cli

    ; use the user data descriptor
    mov eax, 0x2b
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax

    ; I need to edit the stack eflags

    ; push the user data descriptor
    push eax    ; SS

    ; TODO: RESTORE THE TRAPFRAME

    mov eax, user_stack ; [ebp + 4] ; arg1: the user stack
    push eax    ; ESP

    pushfd  ; eflags
    pop eax
    or eax, 0x200
    push eax

    push 0x23 ; CS

    mov eax, task_userspace_asm ; [ebp + 12]
    push eax ; arg2: the user's eip

    iretd
