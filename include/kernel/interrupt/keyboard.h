#ifndef _KEYBOARD_H
#define _KEYBOARD_H

// keycode for PS/2 Keyboard
enum KEYCODE {
    // first five are the ascii equivelent
    KEY_BACKSPACE           = 0x08,
    KEY_TAB                 = 0x09,
    KEY_ENTER               = 0x0d,
    KEY_ESCAPE              = 0x1b,
    KEY_DELETE              = 0x7f,
    // not ascii

    KEY_SHIFT               = 0x80,
    KEY_SHIFT_RELEASE,
    KEY_CAPSLOCK,
    KEY_CAPSLOCK_RELEASE,
    KEY_CTRL,
    KEY_CTRL_RELEASE,
    KEY_ALT,
    KEY_ALT_RELEASE,
    KEY_PRTSCR,
    KEY_NUMLOCK,
    KEY_SCRLOCK,
    KEY_INSERT,
    KEY_END,
    KEY_DOWN,
    KEY_PGDWN,
    KEY_LEFT,
    KEY_CENTER,
    KEY_RIGHT,
    KEY_HOME,
    KEY_UP,
    KEY_PGUP,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_SCANESC             = 0xfe,
    KEY_IGNORE              = 0xff,
};

typedef enum KEYCODE keycode_t;

typedef struct keylayout {
    keycode_t unshift[256];
    keycode_t shift[256];
} keylayout_t;

char line_buff[25]; // buffer for a whole line

void init_keyboard();

#endif
