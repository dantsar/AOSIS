#include <stddef.h>
#include <stdint.h>

#include <interrupt/interrupt.h>
#include <interrupt/keyboard.h>
#include <interrupt/keyboard_lut.h>
#include <common/ringbuff.h>
#include <terminal/tty.h>

static char kb_buff[2048];
static ringbuff_t kb_rbuff = {
    .buff = kb_buff,
    .size = sizeof(kb_buff),
    .head = 0,
    .tail = 0,
    .len  = 0,
};

struct keyboard_status {
    unsigned shift   : 1;
    unsigned alt     : 1;
    unsigned ctrl    : 1;
    unsigned caps    : 1;
} kb_state = {
    .shift = 0,
    .alt   = 0,
    .ctrl  = 0,
    .caps  = 0,
};

char decode_char(keycode_t scancode)
{
    const keycode_t *lookup_table;
    if (kb_state.shift == 1 || kb_state.caps == 1) {
        lookup_table = ps2_layout.shift;
    } else {
        lookup_table = ps2_layout.unshift;
    }

    keycode_t key = lookup_table[scancode];
    if (key == KEY_SHIFT) {
        kb_state.shift = 1;
        return KEY_SHIFT;
    } else if (key == KEY_CAPSLOCK) {
        kb_state.caps = !kb_state.caps;
        return KEY_CAPSLOCK;
    }

    tty_putchar(key);
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
        if (ps2_layout.unshift[scancode] == KEY_SHIFT) {
            kb_state.shift = 0;
        }
        // tty_printstr("RELEASED\n");
    } else {
        uint8_t key = decode_char(scancode);

        // tty_putchar(key);
        // tty_putchar('\n');
        // tty_printstr("PRESSED\n");
    }
}

void init_keyboard() 
{
    idt_handlers[33] = (idt_handler)&key_press;
}
