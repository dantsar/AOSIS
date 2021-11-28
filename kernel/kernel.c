#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <kernel/interrupt/interrupt.h>
#include <kernel/interrupt/pic.h>
#include <kernel/interrupt/keyboard.h>
#include <kernel/terminal/tty.h>
#include <kernel/terminal/vga.h>

extern void div_zero(void);

void kmain() 
{
	initialize_terminal();
	init_interrupt();

	div_zero();

    // asm volatile("int $0x00");
	// if (are_interrupts_enabled()) {
	// 	tty_printstr("Interrupts are enabled\n");
	// }
	// for(;;);

	// tty_printstr("UGH\n");
	// tty_printstr("`Hello there': ");
	// tty_printint(strlen("Hello there"));

    asm volatile("sti");
	asm volatile ("int $0x0");
	// asm volatile ("int $0x1");
	// asm volatile ("int $0x2");
	// asm volatile ("int $0x3");
	// asm volatile ("int $0x4");
	// asm volatile ("int $0x5");
	// asm volatile ("int $0x6");
	// asm volatile ("int $0x7");
	// asm volatile("push $0");
	asm volatile ("int $0x8"); // suss
	// asm volatile ("int $0x9");
	// // asm volatile ("int $0xa"); // suss
	// // asm volatile ("int $0xb"); // suss
	// // asm volatile ("int $0xc"); // suss
	// // asm volatile ("int $0xd"); // suss
	// // asm volatile ("int $0xe"); // suss
	// asm volatile ("int $0xf");
	// asm volatile ("int $0x10");
	// // asm volatile ("int $0x11"); // not that suss
	// asm volatile ("int $0x12");
	// asm volatile ("int $0x13");
	// asm volatile ("int $0x14");
	// asm volatile ("int $0x15");
	// asm volatile ("int $0x16");
	// asm volatile ("int $0x17");
	// asm volatile ("int $0x18");
	// asm volatile ("int $0x19");
	// asm volatile ("int $0x1a");
	// asm volatile ("int $0x1b");
	// asm volatile ("int $0x1c");
	// asm volatile ("int $0x1d");
	// asm volatile ("int $0x1e"); // not that suss
	// asm volatile ("int $0x1f");
	
	// init_timer(19);
	// init_keyboard();
	// asm volatile("sti");

	// tty_printstr("Test timer interrupt\n");
	// for (int i = 0; i < 10; i++)
	// 	asm volatile ("int $32");

	// asm volatile ("int $32");


	// tty_printstr("UH OH\n");

	for(;;); // spin forever
}
