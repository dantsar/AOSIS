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
#include <terminal/shell.h>
#include <terminal/tty.h>
#include <terminal/vga.h>

// // used to test interrupts
// extern void div_zero(void);
// extern void branch_nowhere(void);

// // Rust test
// extern uint32_t add_nums(uint32_t n1, uint32_t n2);

void panic(const char *msg)
{
	if (msg == NULL) {
		kprintf("KERNEL PANIC!!\n");
	} else {
		kprintf("KERNEL PANIC: %s\n", msg);
	}

	for(;;)
		cli();
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
    paging_init();

	kprintf("[-] Initializing Heap...\n");
    kmalloc_init();

    uint32_t *kmalloc_test;
    uint32_t *test_int;

    kprintf("");

    kmalloc_test = (struct test *)kmalloc(8);
    kprintf("kmalloc_test: %x\n", kmalloc_test);

    test_int = (uint32_t *)kmalloc(9);
    kprintf("kmalloc_test: %x\n", test_int);

    // kmalloc_test = (struct test *)kmalloc(24);
    // kprintf("kmalloc_test: %x\n", kmalloc_test);

    // test_int = (uint32_t *)kmalloc(32);
    // kprintf("kmalloc_test: %x\n", test_int);


    // testing kfree
    kprintf("Calling kfree\n");

    kfree(kmalloc_test);
    kfree(test_int);

    kmalloc_test = (struct test *)kmalloc(8);
    kprintf("kmalloc_test: %x\n", kmalloc_test);

    test_int = (uint32_t *)kmalloc(9);
    kprintf("kmalloc_test: %x\n", test_int);



    // kprintf("[-] Initializing Virtual Memory Manager...\n");
    // vmm_init();

    // kprintf("[-] Initializing Scheduler...\n");
	// init_sched

	kprintf("[-] Initializing Keyboard...\n");
	keyboard_init();

	kprintf("[-] Initializing Timer...\n");
	pic_init(20, false);

    // Initialize Last
	kprintf("[-] Launching Kernel Console...\n");
	kconsole();
}
