
#include <task/scheduler.h>
#include <task/task.h>

extern struct task *current_task;

extern void task_switch_asm(struct task *next_task);

void scheduler_init()
{
    // testing creating and switching between tasks
    task_create();
}

void scheduler(void)
{
    task_switch_asm(current_task->next_task);
}
