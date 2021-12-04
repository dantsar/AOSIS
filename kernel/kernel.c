#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <interrupt/interrupt.h>
#include <interrupt/pic.h>
#include <interrupt/keyboard.h>
#include <terminal/tty.h>
#include <terminal/vga.h>

extern void div_zero(void);
extern void branch_nowhere(void);

void kmain() 
{
	initialize_terminal();
	init_interrupt();

	// div_zero(); // should handle exception no. 0 and loop endlessly
	// branch_nowhere(); // branches to an area with no valid code 

	asm volatile("sti");
	
	init_timer(19, false);
	init_keyboard();

	for(;;); // spin forever
}
