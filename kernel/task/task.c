#include <stdint.h>

#include <memory/kmalloc.h>
#include <memory/memory.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <task/task.h>
#include <task/scheduler.h>
#include <terminal/tty.h>

extern struct page_directory *kernel_page_directory;
extern uint32_t stack_top;

struct task *current_task;
struct task *task_list_head;
struct task *task_list_tail;


extern uint32_t task_switch_init_stack_asm(uint32_t kernel_stack_base);

extern void task_switch_asm(struct task *next_thread);


// ---------------- TESTING TASKS ----------------
#include <interrupt/pic.h>

extern void switch_to_usermode_asm(void);

void task_test()
{
    uint32_t previous_tick = 0U; // should be a local variable

    while (1)
    {
        uint32_t tick = pic_get_tick();
        if (tick >= (previous_tick + 20)) // ~5 seconds since tick rate is 20/sec
        {
            previous_tick = tick;

            // kprintf("Task_test: %d (SWITCHING TASK)\n", tick);

            // do something here and print to the screen
        }

        // kprintf("Task_test ticked");
        scheduler();
    }
}

// need to add this to the page table
void task_userspace() __attribute__((section (".multiboot.text")));
uint32_t user_stack[1024]  __attribute((section (".multiboot.data"), aligned (4096)));

void task_userspace(void)
{
    while (1)
    {
        // asm volatile("cli"); // This should trigger a General Protection Fault
    }
}

// ---------------- DONE TESTING TASKS ----------------


// there has been a task running since bootup, and so, create and populate
// task struct for it
void task_init()
{
    struct task *boot_task = (struct task *)kmalloc(sizeof(struct task));

    boot_task->pid               = 0U;
    boot_task->next_task         = boot_task; // circular list
    boot_task->kernel_stack_base = P2V_ADDR(stack_top);
    boot_task->page_directory    = (uint32_t)kernel_page_directory;

    // Boot task's stack does not need to be initialized.

    current_task = boot_task;
    task_list_head = boot_task;
    task_list_tail = boot_task;



    switch_to_usermode_asm();



    // OKAY, with the current kludges, I can just switch into the userspace task and die
}

struct task *task_create(void)
{
    // create the task and append to the task list tail
    struct task *new_task = (struct task *)kmalloc(sizeof(struct task));

    new_task->pid               = task_list_tail->pid + 1;
    new_task->next_task         = task_list_head;
    new_task->page_directory    = (uint32_t)NULL; /* paging_clone_directory() create a new page directory */
    new_task->kernel_stack_base = ((uint32_t)vmm_alloc_page() + PAGE_SIZE);
    new_task->kernel_stack_top  = task_switch_init_stack_asm(new_task->kernel_stack_base);

    task_list_tail->next_task = new_task;
    task_list_tail            = new_task;

    return new_task;
}
