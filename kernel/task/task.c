#include <stdint.h>

#include <memory/kmalloc.h>
#include <task/task.h>

extern struct page_directory *kernel_page_directory;
extern uint32_t stack_top;

// there has been a task running since bootup, and so, create and populate
// task struct for it
void task_init()
{
    struct task *boot_task = (struct task *)kmalloc(sizeof(struct task));

    boot_task->pid              = 0U;
    boot_task->next_task        = NULL;
    boot_task->page_directory   = (uint32_t)kernel_page_directory;
    boot_task->kernel_stack_top = stack_top;

}
