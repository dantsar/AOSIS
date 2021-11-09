#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <stdint.h>

#define IRQ_TIMER       0 
#define IRQ_KEYBOARD    1

// https://wiki.osdev.org/Interrupt_Descriptor_Table
// offset represents the bits of the address of the entry point of the ISR
struct idt_desc {
    uint16_t offset_1;  // offset bits 0..15
    uint16_t selector;  // code segment selector in GDT or LDT
    uint8_t  zero;      // unused and set to zero
    uint8_t  type_attr; // types and attributes
    uint16_t offset_2;  // offset bits 16..31
} __attribute__((packed));

struct idt_pointer {
    uint16_t size;
    uint32_t ptr;
} __attribute__((packed));

extern struct idt_desc idt[];
extern struct idt_pointer idtp;

// initialize and load the IDT
int init_interrupt();

#endif
