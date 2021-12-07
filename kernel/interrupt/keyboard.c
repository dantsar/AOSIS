#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <interrupt/interrupt.h>
#include <interrupt/keyboard.h>
#include <interrupt/keyboard_lut.h>
#include <common/ringbuff.h>
#include <terminal/tty.h>

static uint8_t kb_buff[2048] = {0};
static size_t num_keys = 0;

static void add_key(keycode_t key)
{
    if (key == KEY_BACKSPACE) {
        if (num_keys) num_keys--;

        kb_buff[num_keys] = ' ';
        tty_deleteprev();
    } else if (key == KEY_ENTER) {
        tty_putchar('\n');
        memset(kb_buff, '\n', sizeof(kb_buff));
    } else if (!isprint(key) || !isspace(key)) {
        kb_buff[num_keys++] = key;
        tty_putchar(key);
    }
}

// the status of the keyboard, initialize everything to off
struct {
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

static keycode_t decode_scancode(keycode_t scancode)
{
    const keycode_t *lookup_table;
    if (kb_state.shift == 1 || kb_state.caps == 1) {
        lookup_table = ps2_layout.shift;
    } else {
        lookup_table = ps2_layout.unshift;
    }

    keycode_t key = lookup_table[scancode];
    return key;
}

static void isr_key_press() {
    outb(0x20, 0x20);   // Send EOI

    uint8_t scancode = inb(0x60); // get the scancode from the keyboard

    // 0xe0 indicates that the scancode for the key is the next byte
    if (scancode == 0xe0) {
        return;
    }

    keycode_t key = decode_scancode(scancode);

    // add shortcut to clear the screen
    if ((key == 'l') && kb_state.ctrl == 1) {
        tty_clear();
        return;
    }

    switch (key) {
        case KEY_SHIFT:
            kb_state.shift = 1;
            break;
        case KEY_SHIFT_RELEASE:
            kb_state.shift = 0;
            break;
        case KEY_CAPSLOCK:
            kb_state.caps = !kb_state.caps;
            break;
        case KEY_CTRL:
            kb_state.ctrl = 1;
            break;
        case KEY_CTRL_RELEASE:
            kb_state.ctrl = 0;
            break;
        case KEY_ALT:
            kb_state.alt = 1;
            break;
        case KEY_ALT_RELEASE:
            kb_state.alt = 0;
            break;
        case KEY_DELETE:

            break;
        default:
            add_key(key);
            break;
    }
}

void init_keyboard() 
{
    idt_handlers[33] = (idt_handler)&isr_key_press;
}
