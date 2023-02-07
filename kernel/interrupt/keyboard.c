#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ctype.h>

#include <interrupt/interrupt.h>
#include <interrupt/keyboard.h>
#include <interrupt/keyboard_lut.h>
#include <terminal/tty.h>

struct keyboard keyboard = {
    .len = 0,
    .head = 0,
    .tail = 0,
};

// global state of the keyboard
kb_state_t kb_state = {
    .shift   = 0,
    .alt     = 0,
    .ctrl    = 0,
    .caps    = 0,
    .numlock = 0,
    .scrlock = 0,
    .esc     = 0,
};

bool kb_get_key(struct key *key)
{
    if (keyboard.len == 0) {
        return false;
    }

    if (key) {
        *key = keyboard.buff[keyboard.head];
    }

    keyboard.head = (keyboard.head + 1) % sizeof(keyboard.buff);
    keyboard.len--;
    return true;
}

static void add_key(keycode_t key)
{
    if (keyboard.len < sizeof(keyboard.buff)) {
        struct key new = {
            .key = key,
            .state = kb_state,
        };

        keyboard.len++;
        keyboard.buff[keyboard.tail] = new;
        keyboard.tail = (keyboard.tail + 1) % sizeof(keyboard.buff);
    }
}

static keycode_t decode_scancode(keycode_t scancode)
{
    const keycode_t *lookup_table;
    keycode_t key;

    if (isalpha(ps2_layout.shift[scancode])) {
        lookup_table = (kb_state.shift ^ kb_state.caps) ? ps2_layout.shift : ps2_layout.unshift;
    } else {
        lookup_table = (kb_state.shift) ? ps2_layout.shift : ps2_layout.unshift;
    }

    key = lookup_table[scancode];
    return key;
}

static void isr_key_press() {
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
            // does nothing because arrow keys are currently not handled
            break;
        default:
            add_key(key);
            break;
    }

    outb(0x20, 0x20);   // Send EOI
}

void keyboard_init()
{
    idt_handlers[33] = (idt_handler)&isr_key_press;
}
