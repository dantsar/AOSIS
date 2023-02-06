#pragma once

#include <stdint.h>

enum task_state
{
    NEW = 0,
    READY,
    ACTIVE,
    DONE,
    STOPPED
};

// TODO: clean up the data member, some of these don't belong...
struct task
{
    // These MUST maintain order because of assembly code assumptions
    uint32_t kernel_stack_top; // kind of useless
    struct trapframe *trapframe;
    uint32_t page_directory_phys;

    // The following struct elements can be in any order
    uint32_t pid;
    bool is_user;

    void *page_directory_virt;

    uint32_t kernel_stack_base;
    uint32_t user_stack;
    uint32_t user_stack_top;

    struct task *next_task;
};

struct user_task_info
{
    uint32_t starting_eip;

    uint32_t phys_addr_start;
    uint32_t phys_addr_end;
    uint32_t len;
};

struct trapframe
{
    // Segment
    uint32_t ds;

    // Registers from "pushad"
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t bs_esp; // this register is ignored by popad
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t int_number; // The interrupt number push by all the irq and isr handler stubs defined in "init_idt.asm"

    // Registers pushed to the stack by an interrupt
    uint32_t error_code; // hardware error code
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;

    // Only valid when switching from user space to kernel space
    uint32_t esp;
    uint32_t ss;
};


void task_init(void);

void task_update_trapframe(struct trapframe *tf);

void task_create_kernel(void);

void task_create_user(void);

void task_init_user_task_info(struct user_task_info task);
