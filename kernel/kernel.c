#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <kernel/interrupt/interrupt.h>
#include <kernel/interrupt/pic.h>
#include <kernel/interrupt/keyboard.h>
#include <kernel/terminal/tty.h>
#include <kernel/terminal/vga.h>

extern void div_zero(void);
extern void branch_nowhere(void);

void kmain() 
{
	initialize_terminal();
	init_interrupt();

	// div_zero(); // should handle exception no. 0 and loop endlessly
	// branch_nowhere(); // branches to an area with no valid code 

    // asm volatile("int $0x00");
	// if (are_interrupts_enabled()) {
	// 	tty_printstr("Interrupts are enabled\n");
	// }
	// for(;;);

    asm volatile("sti");
	
	init_timer(19);
	// init_keyboard();
	// asm volatile("sti");

	for(;;); // spin forever
}
