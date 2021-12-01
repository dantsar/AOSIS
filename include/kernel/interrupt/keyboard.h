#ifndef _KEYBOARD_H
#define _KEYBOARD_H

// keycode for PS/2 Keyboard
enum KEYCODE {
    KEY_BACKSPACE = 0x08,
    KEY_TAB       = 0x09,
    KEY_ENTER     = 0x0d,
    KEY_ESCAPE    = 0x1b,
    KEY_CTRL      = 0x81,
    KEY_SHIFT     = 0x2a,
    KEY_ALT       = 0x83,
    KEY_PRTSCR    = 0x90,
    KEY_CAPSLOCK  = 0x91,
    KEY_NUMLOCK   = 0x92,
    KEY_SCRLOCK   = 0x93,
    KEY_INSERT    = 0xa0,
    KEY_END       = 0xa1,
    KEY_DOWN      = 0xa2,
    KEY_PGDWN     = 0xa3,
    KEY_LEFT      = 0xa4,
    KEY_CENTER    = 0xa5,        ///< Keypad Center
    KEY_RIGHT     = 0xa6,
    KEY_HOME      = 0xa7,
    KEY_UP        = 0xa8,
    KEY_PGUP      = 0xa9,
    KEY_DELETE    = 0xaa,
    KEY_MINUS     = 0xab,        ///< Keypad Minus
    KEY_PLUS      = 0xac,        ///< Keypad Plus
    KEY_F1        = 0xb0,
    KEY_F2        = 0xb1,
    KEY_F3        = 0xb2,
    KEY_F4        = 0xb3,
    KEY_F5        = 0xb4,
    KEY_F6        = 0xb5,
    KEY_F7        = 0xb6,
    KEY_F8        = 0xb7,
    KEY_F9        = 0xb8,
    KEY_F10       = 0xb9,
    KEY_F11       = 0xba,
    KEY_F12       = 0xbb,
    KEY_SCANESC   = 0xfe,        ///< Escaped scan code
    KEY_INVALID   = 0xff,        ///< Invalid scan code
};

typedef enum KEYCODE keycode_t;

typedef struct keylayout {
    keycode_t unshift[128];
    keycode_t shift[128];
} keylayout_t;

char line_buff[25]; // buffer for a whole line

void init_keyboard();

#endif
