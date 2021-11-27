#include <stddef.h>
#include <stdint.h>

#include <kernel/interrupt/pic.h>
#include <kernel/interrupt/interrupt.h>
#include <kernel/terminal/tty.h>

extern idt_handler idt_handlers[256];

static uint32_t tick = 0;
static void timer_callback() // struct registers regs
{
   tick++;
   tty_printstr("Tick: ");
   tty_printint(tick);
   tty_putchar('\n');
}

void init_timer(uint32_t frequency)
{
    // setup timer handler
    idt_handlers[32] = (idt_handler) &timer_callback;
    tty_printstr("Timer frequency: ");
    tty_printint(frequency);
    tty_printstr("Hz\n");
    // idt_set_entry(32, (uint32_t) timer_callback, 0x08, 0x8e);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)( (divisor>>8) & 0xFF);

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}
