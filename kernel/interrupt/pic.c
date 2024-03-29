#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <common/common_macros.h>
#include <common/ports.h>
#include <interrupt/pic.h>
#include <interrupt/interrupt.h>
#include <task/scheduler.h>
#include <terminal/tty.h>

extern idt_handler idt_handlers[256];

static bool print = true;

static uint32_t tick = 0;
static void pic_callback(struct idt_registers regs) // struct idt_registers regs
{
    UNUSED(regs);
    tick++;
    if (print)
    {
        tty_printstr("Tick: ");
        tty_printint(tick);
        tty_putc('\n');
    }

    scheduler();
}

uint32_t pic_get_tick(void)
{
    return tick;
}

void pic_init(uint32_t frequency, bool print_tick)
{
    // Enable continuous printing of timer tick
    print = print_tick;

    // setup timer handler
    idt_handlers[32] = (idt_handler)&pic_callback;

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
