#include <stdint.h>
#include <stddef.h>

#include "interrupt.h"
#include "../terminal/tty.h"

struct idt_desc idt[256] = {0};
struct idt_pointer idtp;

extern void _load_idt();

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

int idt_set_entry(uint8_t index, uint32_t handler, uint16_t sel, uint8_t flags) 
{
    // install the address of the handler into the IDT
    idt[index].offset_1 = handler & 0xFFFF;
    idt[index].offset_2 = (handler >> 16) & 0xFFFF;

    idt[index].selector = sel;
    // | 0x60 for usermode
    idt[index].flags = flags;

    return 0;
}

void isr_handler(struct idt_registers regs) 
{
    tty_printstr("Received interrupt: ");
    tty_printint(regs.int_no);
    tty_putchar('\n');
}

void register_interrupt_handler(uint32_t n, idt_handle handler)
{
    idt_handlers[n] = handler;
}

uint32_t tick = 0;
static void timer_callback(struct idt_registers regs)
{
   tick++;
   tty_printstr("Tick: ");
   tty_printint(tick);
   tty_putchar('\n');
}

void init_timer(uint32_t frequency)
{
   // Firstly, register our timer callback.
//    register_interrupt_handler(IRQ0, &timer_callback);
   idt_handlers[32] = &timer_callback;


   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}

int init_interrupt() 
{
    // initialize IDT 
    // memset later
    for (size_t i = 0; i < 256; i++) {
        idt[i] = (struct idt_desc) {
            .offset_1   = (uint16_t) 0,
            .selector   = (uint16_t) 0,
            .zero       = (uint8_t) 0,
            .flags      = (uint8_t) 0,
            .offset_2   = (uint16_t) 0
        };
    }

    // initialize idtp 
    idtp = (struct idt_pointer) {
        .size = (int16_t) sizeof(struct idt_desc) * 256 - 1,
        .ptr = (uint32_t) &idt
    };

    // Remap the irq table.
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    struct idt_cfg {
        uint8_t index;
        uint32_t handler;
        uint8_t select;
        uint8_t flags;
    };
    
    static struct idt_cfg idt_cfg[] = {
        {0,  (uint32_t)isr0,  0x08, 0x8E},
        {1,  (uint32_t)isr1,  0x08, 0x8E},
        {2,  (uint32_t)isr2,  0x08, 0x8E},
        {3,  (uint32_t)isr3,  0x08, 0x8E},
        {4,  (uint32_t)isr4,  0x08, 0x8E},
        {5,  (uint32_t)isr5,  0x08, 0x8E},
        {6,  (uint32_t)isr6,  0x08, 0x8E},
        {7,  (uint32_t)isr7,  0x08, 0x8E},
        {8,  (uint32_t)isr8,  0x08, 0x8E},
        {9,  (uint32_t)isr9,  0x08, 0x8E},
        {10, (uint32_t)isr10, 0x08, 0x8E},
        {11, (uint32_t)isr11, 0x08, 0x8E},
        {12, (uint32_t)isr12, 0x08, 0x8E},
        {13, (uint32_t)isr13, 0x08, 0x8E},
        {14, (uint32_t)isr14, 0x08, 0x8E},
        {15, (uint32_t)isr15, 0x08, 0x8E},
        {16, (uint32_t)isr16, 0x08, 0x8E},
        {17, (uint32_t)isr17, 0x08, 0x8E},
        {18, (uint32_t)isr18, 0x08, 0x8E},
        {19, (uint32_t)isr19, 0x08, 0x8E},
        {20, (uint32_t)isr20, 0x08, 0x8E},
        {21, (uint32_t)isr21, 0x08, 0x8E},
        {22, (uint32_t)isr22, 0x08, 0x8E},
        {23, (uint32_t)isr23, 0x08, 0x8E},
        {24, (uint32_t)isr24, 0x08, 0x8E},
        {25, (uint32_t)isr25, 0x08, 0x8E},
        {26, (uint32_t)isr26, 0x08, 0x8E},
        {27, (uint32_t)isr27, 0x08, 0x8E},
        {28, (uint32_t)isr28, 0x08, 0x8E},
        {29, (uint32_t)isr29, 0x08, 0x8E},
        {30, (uint32_t)isr30, 0x08, 0x8E},
        {31, (uint32_t)isr31, 0x08, 0x8E},
    };

    for (int i = 0; i < 32; i++) {
        idt_set_entry(idt_cfg[i].index, idt_cfg[i].handler,
                      idt_cfg[i].select, idt_cfg[i].flags);
    }

    _load_idt();

    return 0;
}
