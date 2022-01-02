#ifndef _KEYBOARD_LUT_H
#define _KEYBOARD_LUT_H

#include <interrupt/keyboard.h>

// the lookup table for the shifted and unshifted characters 

static const keylayout_t ps2_layout = {
    .unshift    = {
        [0x00] = KEY_IGNORE,
        [0x01] = KEY_ESCAPE,
        [0x02] = '1',
        [0x03] = '2',
        [0x04] = '3',
        [0x05] = '4',
        [0x06] = '5',
        [0x07] = '6',
        [0x08] = '7',
        [0x09] = '8',
        [0x0a] = '9',
        [0x0b] = '0',
        [0x0c] = '-',
        [0x0d] = '=',
        [0x0e] = KEY_BACKSPACE,
        [0x0f] = KEY_TAB,
        [0x10] = 'q',
        [0x11] = 'w',
        [0x12] = 'e',
        [0x13] = 'r',
        [0x14] = 't',
        [0x15] = 'y',
        [0x16] = 'u',
        [0x17] = 'i',
        [0x18] = 'o',
        [0x19] = 'p',
        [0x1a] = '[',
        [0x1b] = ']',
        [0x1c] = KEY_ENTER,
        [0x1d] = KEY_CTRL,
        [0x1e] = 'a',
        [0x1f] = 's',
        [0x20] = 'd',
        [0x21] = 'f',
        [0x22] = 'g',
        [0x23] = 'h',
        [0x24] = 'j',
        [0x25] = 'k',
        [0x26] = 'l',
        [0x27] = ';',
        [0x28] = '\'',
        [0x29] = '`', 
        [0x2a] = KEY_SHIFT,
        [0x2b] = '\\',
        [0x2c] = 'z',
        [0x2d] = 'x', 
        [0x2e] = 'c', 
        [0x2f] = 'v', 
        [0x30] = 'b', 
        [0x31] = 'n', 
        [0x32] = 'm', 
        [0x33] = ',', 
        [0x34] = '.', 
        [0x35] = '/', 
        [0x36] = KEY_SHIFT, 
        [0x37] = '*', 
        [0x38] = KEY_ALT, 
        [0x39] = ' ', 
        [0x3a] = KEY_CAPSLOCK,
        [0x3b] = KEY_F1,
        [0x3c] = KEY_F2,
        [0x3d] = KEY_F3, 
        [0x3e] = KEY_F4, 
        [0x3f] = KEY_F5, 
        [0x40] = KEY_F6, 
        [0x41] = KEY_F7, 
        [0x42] = KEY_F8, 
        [0x43] = KEY_F9,  
        [0x44] = KEY_F10,
        [0x45] = KEY_NUMLOCK, 
        [0x46] = KEY_SCRLOCK, 
        [0x47] = KEY_HOME, 
        [0x48] = KEY_UP, 
        [0x49] = KEY_PGUP, 
        [0x4a] = '-', 
        [0x4b] = KEY_LEFT,
        [0x4c] = KEY_CENTER, 
        [0x4d] = KEY_RIGHT, 
        [0x4e] = '+', 
        [0x4f] = KEY_END, 
        [0x50] = KEY_DOWN, 
        [0x51] = KEY_PGDWN, 
        [0x52] = KEY_INSERT, 
        [0x53] = KEY_DELETE, 
        [0x54] = KEY_IGNORE, 
        [0x55] = KEY_IGNORE, 
        [0x56] = KEY_IGNORE, 
        [0x57] = KEY_F11, 
        [0x58] = KEY_F12, 
        [0x59] = KEY_IGNORE,
        [0x5a] = KEY_IGNORE, 
        [0x5b] = KEY_IGNORE, 
        [0x5c] = KEY_IGNORE, 
        [0x5d] = KEY_IGNORE, 
        [0x5e] = KEY_IGNORE, 
        [0x5f] = KEY_SCANESC, 
        [0x60] = KEY_SCANESC, 
        [0x61] = KEY_IGNORE, 
        [0x62] = KEY_IGNORE, 
        [0x63] = KEY_IGNORE, 
        [0x64] = KEY_IGNORE, 
        [0x65] = KEY_IGNORE, 
        [0x66] = KEY_IGNORE, 
        [0x67] = KEY_IGNORE, 
        [0x68] = KEY_IGNORE, 
        [0x69] = KEY_IGNORE, 
        [0x6a] = KEY_IGNORE, 
        [0x6b] = KEY_IGNORE, 
        [0x6c] = KEY_IGNORE, 
        [0x6d] = KEY_IGNORE, 
        [0x6e] = KEY_IGNORE, 
        [0x6f] = KEY_IGNORE, 
        [0x70] = KEY_IGNORE, 
        [0x71] = KEY_IGNORE, 
        [0x72] = KEY_IGNORE, 
        [0x73] = KEY_IGNORE, 
        [0x74] = KEY_IGNORE, 
        [0x75] = KEY_IGNORE, 
        [0x76] = KEY_IGNORE, 
        [0x77] = KEY_IGNORE, 
        [0x78] = KEY_IGNORE, 
        [0x79] = KEY_IGNORE, 
        [0x7a] = KEY_IGNORE, 
        [0x7b] = KEY_IGNORE, 
        [0x7c] = KEY_IGNORE, 
        [0x7d] = KEY_IGNORE, 
        [0x7e] = KEY_IGNORE,
        [0x9D] = KEY_CTRL_RELEASE,
        [0xaa] = KEY_SHIFT_RELEASE,
        [0xb6] = KEY_SHIFT_RELEASE,
        [0xb8] = KEY_ALT_RELEASE,
    },   
    .shift      = {
        [0x00] = KEY_IGNORE,
        [0x01] = KEY_ESCAPE,
        [0x02] = '!',
        [0x03] = '@',
        [0x04] = '#',
        [0x05] = '$',
        [0x06] = '%',
        [0x07] = '^',
        [0x08] = '&',
        [0x09] = '*',
        [0x0a] = '(',
        [0x0b] = ')',
        [0x0c] = '_',
        [0x0d] = '+',
        [0x0e] = KEY_BACKSPACE,
        [0x0f] = KEY_TAB,
        [0x10] = 'Q',
        [0x11] = 'W',
        [0x12] = 'E',
        [0x13] = 'R',
        [0x14] = 'T',
        [0x15] = 'Y',
        [0x16] = 'U',
        [0x17] = 'I',
        [0x18] = 'O',
        [0x19] = 'P',
        [0x1a] = '{',
        [0x1b] = '}',
        [0x1c] = KEY_ENTER,
        [0x1d] = KEY_CTRL,
        [0x1e] = 'A',
        [0x1f] = 'S',
        [0x20] = 'D',
        [0x21] = 'F',
        [0x22] = 'G',
        [0x23] = 'H',
        [0x24] = 'J',
        [0x25] = 'K',
        [0x26] = 'L',
        [0x27] = ':',
        [0x28] = '"',
        [0x29] = '~', 
        [0x2a] = KEY_SHIFT,
        [0x2b] = '|',
        [0x2c] = 'Z',
        [0x2d] = 'X', 
        [0x2e] = 'C', 
        [0x2f] = 'V', 
        [0x30] = 'B', 
        [0x31] = 'N', 
        [0x32] = 'M', 
        [0x33] = '<', 
        [0x34] = '>', 
        [0x35] = '?', 
        [0x36] = KEY_SHIFT, 
        [0x37] = '*', 
        [0x38] = KEY_ALT, 
        [0x39] = ' ', 
        [0x3a] = KEY_CAPSLOCK,
        [0x3b] = KEY_F1,
        [0x3c] = KEY_F2,
        [0x3d] = KEY_F3, 
        [0x3e] = KEY_F4, 
        [0x3f] = KEY_F5, 
        [0x40] = KEY_F6, 
        [0x41] = KEY_F7, 
        [0x42] = KEY_F8, 
        [0x43] = KEY_F9,  
        [0x44] = KEY_F10,
        [0x45] = KEY_NUMLOCK, 
        [0x46] = KEY_SCRLOCK, 
        [0x47] = KEY_HOME, 
        [0x48] = KEY_UP, 
        [0x49] = KEY_PGUP, 
        [0x4a] = '-', 
        [0x4b] = KEY_LEFT,
        [0x4c] = KEY_CENTER, 
        [0x4d] = KEY_RIGHT, 
        [0x4e] = '+', 
        [0x4f] = KEY_END, 
        [0x50] = KEY_DOWN, 
        [0x51] = KEY_PGDWN, 
        [0x52] = KEY_INSERT, 
        [0x53] = KEY_DELETE, 
        [0x54] = KEY_IGNORE, 
        [0x55] = KEY_IGNORE, 
        [0x56] = KEY_IGNORE, 
        [0x57] = KEY_F11, 
        [0x58] = KEY_F12, 
        [0x59] = KEY_IGNORE,
        [0x5a] = KEY_IGNORE, 
        [0x5b] = KEY_IGNORE, 
        [0x5c] = KEY_IGNORE, 
        [0x5d] = KEY_IGNORE, 
        [0x5e] = KEY_IGNORE, 
        [0x5f] = KEY_SCANESC, 
        [0x60] = KEY_SCANESC, 
        [0x61] = KEY_IGNORE, 
        [0x62] = KEY_IGNORE, 
        [0x63] = KEY_IGNORE, 
        [0x64] = KEY_IGNORE, 
        [0x65] = KEY_IGNORE, 
        [0x66] = KEY_IGNORE, 
        [0x67] = KEY_IGNORE, 
        [0x68] = KEY_IGNORE, 
        [0x69] = KEY_IGNORE, 
        [0x6a] = KEY_IGNORE, 
        [0x6b] = KEY_IGNORE, 
        [0x6c] = KEY_IGNORE, 
        [0x6d] = KEY_IGNORE, 
        [0x6e] = KEY_IGNORE, 
        [0x6f] = KEY_IGNORE, 
        [0x70] = KEY_IGNORE, 
        [0x71] = KEY_IGNORE, 
        [0x72] = KEY_IGNORE, 
        [0x73] = KEY_IGNORE, 
        [0x74] = KEY_IGNORE, 
        [0x75] = KEY_IGNORE, 
        [0x76] = KEY_IGNORE, 
        [0x77] = KEY_IGNORE, 
        [0x78] = KEY_IGNORE, 
        [0x79] = KEY_IGNORE, 
        [0x7a] = KEY_IGNORE, 
        [0x7b] = KEY_IGNORE, 
        [0x7c] = KEY_IGNORE, 
        [0x7d] = KEY_IGNORE, 
        [0x7e] = KEY_IGNORE,
        [0x9D] = KEY_CTRL_RELEASE,
        [0xaa] = KEY_SHIFT_RELEASE,
        [0xb6] = KEY_SHIFT_RELEASE,
        [0xb8] = KEY_ALT_RELEASE,
    },
};

#endif
