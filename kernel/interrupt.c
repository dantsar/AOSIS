#include <stddef.h>
#include "interrupt.h"

struct idt_desc idt[256] = {0};
struct idt_pointer idtp;

extern void _load_idt();

int init_interrupt() 
{
    // initialize IDT 
    for (size_t i = 0; i < 256; i++) {
        idt[i] = (struct idt_desc) {
            .offset_1 = 0,
            .selector = 0,
            .zero = (uint8_t) 0,
            .type_attr = (uint8_t) 0,
            .offset_2 = (uint16_t) 0,
        };
    }

    // initialize idtp 
    idtp = (struct idt_pointer) {
        .size = (int16_t) sizeof(idt),
        .ptr = (uint32_t) &idt
    };

    _load_idt();

    return 0;
}
