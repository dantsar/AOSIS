#include <stdbool.h>
#include <stddef.h>

#include <common/cpu.h>
#include <memory/gdt.h>
#include <task/scheduler.h>
#include <task/task.h>

extern struct task *current_task;
extern struct tss tss_region;

extern void task_switch_asm(struct task *next_task);
extern void task_switch_to_usermode_asm(uint32_t user_stack);

static bool scheduler_initialized = false;

void scheduler_init()
{
    scheduler_initialized = true;
}

void scheduler(void)
{
    if (!scheduler_initialized)
    {
        return;
    }

    if (current_task == NULL || current_task->next_task == NULL)
    {
        return;
    }

    task_switch_asm(current_task->next_task);

    // // // I'm not sure about this, the trapframe should already be created by this point
    // // if (current_task->is_user)
    // // {
    // //     // current_task->user_stack_top = tss_region->esp
    // // }

    // struct task *next_task = current_task->next_task;

    // if (!next_task->is_user)
    // {
    //     // task_switch_asm(next_task);
    // }
    // else
    // {
    //     // TODO: change the tss esp0 to the user process' kernel stack
    //     // gdt_update_tss(next_task->kernel_stack_base); // NOTE: the kernel stack is always empty when returning to userspace
    //     // task_switch_to_usermode_asm(0U); /* trapframe */ // the argument doesn't make any sense TODO: fix
    //     return;
    // }
}
