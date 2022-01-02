#include <stdint.h>
#include <stddef.h>

#include <interrupt/interrupt.h>
#include <terminal/tty.h>

struct idt_desc idt[256] = {0};
struct idt_pointer idtp;

extern void _load_idt();

const char *int_msgs[] = {
    "Divide by Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "CoProcessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "Floatint-Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Pointe Exception",
    "Virtualziation Exception",
    "Control Protection Exception", // 21
    "Reserved", "Reserved","Reserved","Reserved","Reserved","Reserved",
    "Reserved","Reserved","Reserved","Reserved",
    /* IRQs Afterwards */
};

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

void isr_handler(registers_t regs) 
{
    // TO DO: check for certain interrupts
    static unsigned prev_int = -1;
    if (regs.int_no == prev_int) { 
        tty_printstr("...");
        for(;;);
    } else {
        prev_int = regs.int_no;
    }

    tty_printstr("Received interrupt: ");
    tty_printint(regs.int_no);
    tty_printstr(" (");
    tty_printstr(int_msgs[regs.int_no]);
    tty_printstr(")\n");

    if (idt_handlers[regs.int_no] != NULL) {
        tty_printstr("HANLDING INTERRUPT");
        idt_handler handle = idt_handlers[regs.int_no];
        handle(regs);
        tty_printstr("HANDLED INTERRUPT");
    }
}

void irq_handler(registers_t regs) 
{
    // if (regs.int_no != 32) {
    //     tty_printstr("IRQ handler\n");
    //     tty_printstr("int no: ");
    //     tty_printint(regs.int_no);
    //     tty_putchar('\n');
    // }

    // If interrupt from slave PIC, send reset signal to slave
    if (regs.int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    // Send reset signal to master. 
    outb(0x20, 0x20);

    if (idt_handlers[regs.int_no] != NULL)
    {
        idt_handler handle = idt_handlers[regs.int_no];
        handle(regs);
    }
}

void other_interrupt(registers_t regs) {
    tty_printstr("???SOMEOTHER INTERRUPT???\n");
    tty_printint(regs.int_no);
}

int init_interrupt() 
{
    // initialize IDT and idt_handlers to NULL
    // memset later
    for (size_t i = 0; i < 256; i++) {
        idt[i] = (struct idt_desc) {
            .offset_1   = (uint16_t) 0,
            .selector   = (uint16_t) 0,
            .zero       = (uint8_t) 0,
            .flags      = (uint8_t) 0,
            .offset_2   = (uint16_t) 0
        };
        idt_handlers[i] = NULL;
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

    struct idt_config{
        uint8_t index;
        uint32_t handler;
        uint8_t select;
        uint8_t flags;
    };
    
    static struct idt_config idt_cfg[] = {
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
        {32, (uint32_t)irq0,  0x08, 0x8E},
        {33, (uint32_t)irq1,  0x08, 0x8E},
        {34, (uint32_t)irq2,  0x08, 0x8E},
        {35, (uint32_t)irq3,  0x08, 0x8E},
        {36, (uint32_t)irq4,  0x08, 0x8E},
        {37, (uint32_t)irq5,  0x08, 0x8E},
        {38, (uint32_t)irq6,  0x08, 0x8E},
        {39, (uint32_t)irq7,  0x08, 0x8E},
        {40, (uint32_t)irq8,  0x08, 0x8E},
        {41, (uint32_t)irq9,  0x08, 0x8E},
        {42, (uint32_t)irq10, 0x08, 0x8E},
        {43, (uint32_t)irq11, 0x08, 0x8E},
        {44, (uint32_t)irq12, 0x08, 0x8E},
        {45, (uint32_t)irq13, 0x08, 0x8E},
        {46, (uint32_t)irq14, 0x08, 0x8E},
        {47, (uint32_t)irq15, 0x08, 0x8E},
    };
    int last = 47;
    for (int i = 0; i <= last; i++) {
        idt_set_entry(idt_cfg[i].index, idt_cfg[i].handler,
                      idt_cfg[i].select, idt_cfg[i].flags);
    }

    for (int i = last; i < 256; i++) {
        idt_handlers[i] = (idt_handler) other_interrupt;
        idt_set_entry(i, (uint32_t)other_interrupt, 0x08, 0x8E);
    }

    _load_idt();

	asm volatile("sti");

    return 0;
}
