#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <stdint.h>

#define IRQ_TIMER       0 
#define IRQ_KEYBOARD    1

// https://wiki.osdev.org/Interrupt_Descriptor_Table
// offset represents the bits of the address of the entry point of the ISR
// size of idt descriptor is 64
struct idt_desc {
    uint16_t offset_1;  // offset bits 0..15
    uint16_t selector;  // code segment selector in GDT or LDT
    uint8_t  zero;      // unused and set to zero
    uint8_t  flags; // types and attributes
    uint16_t offset_2;  // offset bits 16..31
} __attribute__((packed));

struct idt_pointer {
    uint16_t size;
    uint32_t ptr;
} __attribute__((packed));

extern struct idt_desc idt[];
extern struct idt_pointer idtp;

typedef struct idt_registers
{
    // unsigned int gs, fs, es, ds;      /* pushed the segs last */
    uint32_t ds;                  // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} idt_regs;

typedef void (*idt_handle)(struct idt_registers);
idt_handle idt_handlers[256]; 

// sets the specified entry in the IDT
int idt_set_entry(uint8_t index, uint32_t handler, uint16_t sel, uint8_t flags);

// initialize and load the IDT
int init_interrupt();

// generic handler
void isr_handler(struct idt_registers regs);

// initalize the PIT timer
void init_timer(uint32_t frequency);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

#endif
