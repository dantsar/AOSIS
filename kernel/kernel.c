#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ctype.h>
#include <string.h>

#include <common/common_macros.h>
#include <common/cpu.h>
#include <kernel.h>
#include <interrupt/interrupt.h>
#include <interrupt/pic.h>
#include <interrupt/keyboard.h>
#include <memory/kmalloc.h>
#include <memory/multiboot.h>
#include <memory/paging.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <task/task.h>
#include <task/scheduler.h>
#include <terminal/shell.h>
#include <terminal/tty.h>
#include <terminal/vga.h>

// // used to test interrupts
// extern void div_zero(void);
// extern void branch_nowhere(void);

// // Rust test
// extern uint32_t add_nums(uint32_t n1, uint32_t n2);

extern void gdt_load_tss_asm(void);

void panic(const char *msg)
{
    if (msg == NULL) {
        kprintf("KERNEL PANIC!!\n");
    } else {
        kprintf("KERNEL PANIC: %s\n", msg);
    }

    for(;;)
    {
        cli();
    }
}

void kmain(struct multiboot_info *mbt, uint32_t magic)
{
    initialize_terminal();
    kprintf("[-] Terminal Initalized\n");

    kprintf("[-] Initializing Interrupts...\n");
    interrupt_init();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid bootloader magic number");
    }

    // Memory Management
    kprintf("[-] Initializing Physical Memory Management...\n");
    pmm_init(mbt);

    kprintf("[-] Initializing Paging and Virtual Memory...\n");
    void *initial_page_table = paging_init();

    kprintf("[-] Initializing Heap...\n");
    kmalloc_init();

    kprintf("[-] Initializing Virtual Memory Manager...\n");
    vmm_init(initial_page_table);

    kprintf("[-] Initializing Keyboard...\n");
    keyboard_init();

    kprintf("[-] Initializing Timer...\n");
    pic_init(20, false);

    gdt_load_tss_asm();

    kprintf("[-] Initializing Scheduler...\n");
    scheduler_init();

    // Testing task creation
    // task_create_user();
    // task_init();

    // Initialize Last
    kprintf("[-] Launching Kernel Console...\n");
    kshell();
}
