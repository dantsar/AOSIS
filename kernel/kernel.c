#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ctype.h>
#include <string.h>

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
		tty_printstr("KERNEL PANIC!!\n");
	} else {
		kprintf("KERNEL PANIC: %s\n", msg);
	}

	for(;;)
		cli();
}

void kmain(multiboot_info_t *mbt, uint32_t magic)
{
	initialize_terminal();
	tty_printstr("[-] Terminal Initalized\n");

	tty_printstr("[-] Initializing Interrupts...\n");
	interrupt_init();

	tty_printstr("[-] Initializing Keyboard...\n");
	keyboard_init();

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		panic("invalid bootloader magic number");
	}

	// memory management
	tty_printstr("[-] Initializing Physical Memory Mapping...\n");
	pmm_init(mbt);

	// tty_printstr("[-] Initializing Virtual Memory Mapping...\n");
	// vmm_init
	// tty_printstr("[-] Initializing Scheduler...\n");
	// init_sched

	tty_printstr("[-] Initializing Interrupts...\n");
	interrupt_init();

	tty_printstr("[-] Initializing Keyboard...\n");
	keyboard_init();

	tty_printstr("[-] Initializing Timer...\n");
	timer_init(20, false);

    // kprintf("add_nums: %d\n", add_nums(1U, 2U));

    // Initialize Last
	tty_printstr("[-] Launching Kernel Console...\n");
	kconsole();
}
