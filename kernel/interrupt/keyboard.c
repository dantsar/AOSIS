#include <stddef.h>
#include <stdint.h>

#include <kernel/interrupt/keyboard.h>
#include <kernel/interrupt/keyboard_lut.h>
#include <kernel/interrupt/interrupt.h>
#include <kernel/terminal/tty.h>

uint8_t last_char = 0;

struct keyboard_status {
    unsigned shift   : 1;
    unsigned alt     : 1;
    unsigned ctrl    : 1;
    unsigned caps    : 1;
} kb_state;

char decode_char(keycode_t scancode)
{
    // tty_printstr("Scancode: ");
    // tty_printhex(scancode);
    // tty_putchar('\n');

    if (scancode == KEY_SHIFT) {
        kb_state.shift = 1;
        return KEY_SHIFT;
    }

    const keycode_t *lookup_table;
    if (kb_state.shift == 1) {
        lookup_table = ps2_layout.shift;
    } else {
        lookup_table = ps2_layout.unshift;
    }

    char key = lookup_table[scancode];


    // tty_printhex(scancode);
    // tty_putchar(' ');
    // tty_printhex(key);
    // tty_putchar(' ');
    // tty_putchar(key);
    // tty_putchar('\n');

    return key;
}

static void key_press() {
    outb(0x20, 0x20);   // Send EOI

    // tty_printstr("Key pressed\n");
    // tty_printstr("KB INT NO: ");
    // tty_printint(reg.int_no);
    // tty_putchar('\n');

    uint8_t scancode = inb(0x60);
    if (scancode & 0x80) {
        if (scancode == KEY_SHIFT) {
            kb_state.shift = 0;
        }
        // tty_printstr("RELEASED\n");
    } else {
        uint8_t key = decode_char(scancode);
        tty_putchar(key);
        // tty_putchar(key);
        // tty_putchar('\n');
        // tty_printstr("PRESSED\n");
    }
}

void init_keyboard() 
{
    idt_handlers[33] = (idt_handler)&key_press;
}
