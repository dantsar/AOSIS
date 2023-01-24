#include <stdbool.h>

#include <task/scheduler.h>
#include <task/task.h>

extern struct task *current_task;

extern void task_switch_asm(struct task *next_task);

static bool scheduler_initialized = false;

void scheduler_init()
{
    // testing creating and switching between tasks
    task_create();

    scheduler_initialized = true;
}

void scheduler(void)
{
    if (!scheduler_initialized)
    {
        return;
    }

    task_switch_asm(current_task->next_task);
}
