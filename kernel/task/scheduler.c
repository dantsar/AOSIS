#include <stdbool.h>
#include <stddef.h>

#include <common/cpu.h>
#include <memory/gdt.h>
#include <memory/paging.h>
#include <task/scheduler.h>
#include <task/task.h>

extern struct task *current_task;
extern struct tss tss_region;

extern void task_switch_asm(struct task *next_task);

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

    // paging_flush_tlb();

    struct task *next_task = current_task->next_task;

    if (next_task->is_user)
    {
        gdt_update_tss(next_task->kernel_stack_base);
        // paging_change_page_directory(next_task->page_directory_phys);
    }

    task_switch_asm(current_task->next_task);
}
