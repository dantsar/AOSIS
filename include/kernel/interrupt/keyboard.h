#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdbool.h>

// keycode for PS/2 Keyboard
enum KEYCODE {
    KEY_BACKSPACE           = '\b',
    KEY_TAB                 = '\t',
    KEY_ENTER               = '\n',
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
    KEY_SCANESC,
    KEY_IGNORE,
};

typedef enum KEYCODE keycode_t;

// used for the look up table
typedef struct keylayout {
    keycode_t unshift[256];
    keycode_t shift[256];
} keylayout_t;

typedef struct keyboard_state {
    unsigned shift   : 1;
    unsigned alt     : 1;
    unsigned ctrl    : 1;
    unsigned caps    : 1;
    unsigned numlock : 1;
    unsigned scrlock : 1;
    unsigned esc     : 1;
} kb_state_t;

typedef struct key {
    kb_state_t state;
    keycode_t key;
} key_t;

typedef struct keyboard {
    key_t buff[4096];
    size_t len;
    size_t head, tail;
} keyboard_t;

// the global keyboard state
extern keyboard_t keyboard;

void init_keyboard();

bool kb_get_key(key_t *key);

#endif
