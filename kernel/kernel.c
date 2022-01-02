#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ctype.h>
#include <string.h>

#include <interrupt/interrupt.h>
#include <interrupt/pic.h>
#include <interrupt/keyboard.h>
#include <terminal/shell.h>
#include <terminal/tty.h>
#include <terminal/vga.h>

// used to test interrupts
extern void div_zero(void);
extern void branch_nowhere(void);

void kmain() 
{
	initialize_terminal();

	if (!isprint('a')) {
		tty_printstr("a is printable\n");
	}

	// memory management
	// tty_printstr("[-] Initializing Physical Memory Mapping...\n");
	// init_pmm
	// tty_printstr("[-] Initializing Virtual Memory Mapping...\n");
	// init_vmm
	// tty_printstr("[-] Initializing Scheduler...\n");
	// init_sched


	tty_printstr("[-] Initializing Interrupts...\n");
	init_interrupt();
	
	tty_printstr("[-] Initializing Keyboard...\n");
	init_keyboard();

	tty_printstr("[-] Initializing Timer...\n");
	init_timer(20, false);

	tty_printstr("[-] Launching Kernel Console...\n");
	kconsole();
}
