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
#include <memory/multiboot.h>
#include <memory/pmm.h>
#include <terminal/shell.h>
#include <terminal/tty.h>
#include <terminal/vga.h>

// used to test interrupts
extern void div_zero(void);
extern void branch_nowhere(void);

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
    UNUSED(mbt);

	initialize_terminal();
	kprintf("[-] Terminal Initalized\n");

	kprintf("[-] Initializing Interrupts...\n");
	interrupt_init();

	kprintf("[-] Initializing Keyboard...\n");
	keyboard_init();

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		panic("invalid bootloader magic number");
	}

	// memory management
	// kprintf("[-] Initializing Physical Memory Mapping...\n");
	// pmm_init(mbt);

	// kprintf("[-] Initializing Virtual Memory Mapping...\n");
	// vmm_init
	// kprintf("[-] Initializing Scheduler...\n");
	// init_sched

	kprintf("[-] Initializing Interrupts...\n");
	interrupt_init();

	kprintf("[-] Initializing Keyboard...\n");
	keyboard_init();

	kprintf("[-] Initializing Timer...\n");
	pic_init(20, false);

    // kprintf("add_nums: %d\n", add_nums(1U, 2U));

    // Initialize Last
	kprintf("[-] Launching Kernel Console...\n");
	kconsole();
}
