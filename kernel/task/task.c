#include <stdint.h>

#include <string.h>

#include <memory/gdt.h>
#include <memory/kmalloc.h>
#include <memory/memory.h>
#include <memory/paging.h>
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

extern void task_switch_asm(struct task *next_thread);

static struct user_task_info user_task;

// ---------------- TESTING TASKS ----------------
#include <interrupt/pic.h>

void task_test()
{
    uint32_t previous_tick = 0U; // should be a local variable

    while (1)
    {
        uint32_t tick = pic_get_tick();
        if (tick >= (previous_tick + 2)) // ~5 seconds since tick rate is 20/sec
        {
            previous_tick = tick;

            kprintf("Task_test: %d (SWITCHING TASK)\n", tick);

            // do something here and print to the screen
        }

        // kprintf("Task_test ticked");
        // scheduler();
    }
}

// need to add this to the page table
void task_userspace() __attribute__((section (".multiboot.text")));
void task_userspace(void)
{
    // asm volatile("cli"); // This should trigger a General Protection Fault
    for (;;);

}

// ---------------- DONE TESTING TASKS ----------------

// there has been a task running since bootup, and so, create and populate
// task struct for it
void task_init()
{
    struct task *boot_task = (struct task *)kmalloc(sizeof(struct task));

    boot_task->pid                 = 0U;
    boot_task->next_task           = boot_task; // circular list
    boot_task->page_directory_phys = (uint32_t)kernel_page_directory; // TODO: fix this so it's actually the physical address

    // Boot task's stack does not need to be initialized.

    current_task   = boot_task;
    task_list_head = boot_task;
    task_list_tail = boot_task;
}

void task_update_trapframe(struct trapframe *tf)
{
    if (current_task != NULL)
    {
        current_task->trapframe = tf;
    }
}

void task_create_kernel(void) // TODO: as an argument, take the address to the stating function
{
    // create the task and append to the task list tail
    struct task *new_task = (struct task *)kmalloc(sizeof(struct task));

    new_task->next_task           = task_list_head; // Circular linked list
    new_task->page_directory_phys = (uint32_t)kernel_page_directory; // TODO: figure out if this is right...

    void *alloc_stack = vmm_alloc_page();
    memset(alloc_stack, 0U, PAGE_SIZE);

    new_task->kernel_stack_base = (uint32_t)alloc_stack + PAGE_SIZE;


    new_task->trapframe = (struct trapframe *)(new_task->kernel_stack_base - (sizeof(struct trapframe) - 8)); // -8 because there are no user elements

    // Set the initial trapframe
    new_task->trapframe->eip    = (uint32_t)&task_test;
    new_task->trapframe->eflags = 0x2U;
    new_task->trapframe->cs     = GDT_KERNEL_CODE_SEG;
    new_task->trapframe->ds     = GDT_KERNEL_DATA_SEG;

    if (task_list_tail != NULL)
    {
        new_task->pid             = task_list_tail->pid + 1;
        task_list_tail->next_task = new_task;
    }

    task_list_tail = new_task;
}

void task_create_user(void)
{
    // create the task and append to the task list tail
    struct task *new_task = (struct task *)kmalloc(sizeof(struct task));

    new_task->is_user             = true;
    new_task->next_task           = task_list_head; // Circular linked list
    // new_task->page_directory_phys = paging_create_userspace(&user_task);

    new_task->kernel_stack_base   = ((uint32_t)vmm_alloc_page() + PAGE_SIZE);
    memset((void *)(new_task->kernel_stack_base - PAGE_SIZE), 0, PAGE_SIZE);  // clear starting stack

    new_task->trapframe = (struct trapframe *)(new_task->kernel_stack_base - (sizeof(struct trapframe)));

    // Set the initial trapframe
    new_task->trapframe->eip    = (uint32_t)&task_userspace; // (uint32_t)user_task.starting_eip;
    new_task->trapframe->eflags = 0x2U;
    new_task->trapframe->cs     = GDT_USER_CODE_SEG;
    new_task->trapframe->ds     = GDT_USER_DATA_SEG;
    new_task->trapframe->ss     = GDT_USER_DATA_SEG;
    new_task->trapframe->esp    = KERNEL_VIRT_BASE;

    if (task_list_tail != NULL)
    {
        new_task->pid             = task_list_tail->pid + 1;
        task_list_tail->next_task = new_task;
    }

    task_list_tail = new_task;
}

void task_init_user_task_info(struct user_task_info user_task_info)
{
    static bool once = true;

    if (once)
    {
        user_task = user_task_info;
        once = false;
    }
}
