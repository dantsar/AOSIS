#ifndef _KEYBOARD_H
#define _KEYBOARD_H

// keycode for PS/2 Keyboard
enum KEYCODE {
    KEY_BACKSPACE           = 0x08,
    KEY_TAB                 = 0x09,
    KEY_ENTER               = 0x0d,
    KEY_ESCAPE              = 0x1b,
    KEY_DELETE              = 0x7f,
    KEY_SHIFT               = 0x80,
    KEY_SHIFT_RELEASE       = 0x81,
    KEY_CAPSLOCK            = 0x82,
    KEY_CAPSLOCK_RELEASE    = 0x83,
    KEY_CTRL                = 0x83,
    KEY_CTRL_RELEASE        = 0x83,
    KEY_ALT                 = 0x84,
    KEY_ALT_RELEASE         = 0x85,
    KEY_PRTSCR              = 0x86,
    KEY_NUMLOCK             = 0x87,
    KEY_SCRLOCK             = 0x88,
    KEY_INSERT              = 0x89,
    KEY_END                 = 0x90,
    KEY_DOWN                = 0x91,
    KEY_PGDWN               = 0x92,
    KEY_LEFT                = 0x93,
    KEY_CENTER              = 0x94,
    KEY_RIGHT               = 0x95,
    KEY_HOME                = 0x96,
    KEY_UP                  = 0x97,
    KEY_PGUP                = 0x98,
    KEY_F1                  = 0xb0,
    KEY_F2                  = 0xb1,
    KEY_F3                  = 0xb2,
    KEY_F4                  = 0xb3,
    KEY_F5                  = 0xb4,
    KEY_F6                  = 0xb5,
    KEY_F7                  = 0xb6,
    KEY_F8                  = 0xb7,
    KEY_F9                  = 0xb8,
    KEY_F10                 = 0xb9,
    KEY_F11                 = 0xba,
    KEY_F12                 = 0xbb,
    KEY_SCANESC             = 0xfe,
    KEY_IGNORE              = 0xff,
};

typedef enum KEYCODE keycode_t;

typedef struct keylayout {
    keycode_t unshift[128];
    keycode_t shift[128];
} keylayout_t;

char line_buff[25]; // buffer for a whole line

void init_keyboard();

#endif
