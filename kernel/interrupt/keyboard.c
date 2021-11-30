#include <stddef.h>
#include <stdint.h>

#include <kernel/interrupt/keyboard.h>
#include <kernel/interrupt/interrupt.h>
#include <kernel/terminal/tty.h>

uint8_t last_char = 0;

#define BSP KEY_BACKSPACE
#define TAB KEY_TAB
#define ENT KEY_ENTER
#define ESC KEY_ESCAPE
#define CTL KEY_CTRL
#define SHF KEY_SHIFT
#define ALT KEY_ALT
#define PRS KEY_PRTSCR
#define CAP KEY_CAPSLOCK
#define NML KEY_NUMLOCK
#define SCL KEY_SCRLOCK
#define INS KEY_INSERT
#define END KEY_END
#define DWN KEY_DOWN
#define PGD KEY_PGDN
#define LFT KEY_LEFT
#define CNT KEY_CENTER
#define RGT KEY_RIGHT
#define HME KEY_HOME
#define KUP KEY_UP
#define PUP KEY_PGUP
#define DEL KEY_DEL
#define MIN KEY_MINUS
#define PLS KEY_PLUS
#define F_1 KEY_F1
#define F_2 KEY_F2
#define F_3 KEY_F3
#define F_4 KEY_F4
#define F_5 KEY_F5
#define F_6 KEY_F6
#define F_7 KEY_F7
#define F_8 KEY_F8
#define F_9 KEY_F9
#define F10 KEY_F1
#define F11 KEY_F1
#define F12 KEY_F1
#define SES KEY_SCANESC
#define INV KEY_INVALID

static const keylayout_t ps2_layout = {
    .unshift    = {
        INV, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BSP,
        TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENT, CTL, 
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'','`', SHF, '\\','z',
        'x', 'c', 'v', 'b', 'n', 'm', ',', '.', SHF, PRS, ALT, ' ', CAP, F_1, F_2,
        F_3, F_4, F_5, F_6, F_7, F_8, F_9, F10, NML, SCL, HME, KUP, PUP, MIN, LFT,
        CNT, RGT, PLS, END, DWN, PGD, INS, DEL, INV, INV, INV, F11, F12, INV, INV,
        INV, INV, INV, INV, INV, SES, SES, INV, INV, INV, INV, INV, INV, INV, INV, 
        INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, 
        INV, INV, INV, INV, INV, INV, INV,
    },
    .shift      = {
        INV, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BSP,
        TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', ENT, CTL, 
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"','~', SHF, '|', 'Z',
        'X', 'C', 'V', 'B', 'N', 'M', '<', '>', SHF, PRS, ALT, ' ', CAP, F_1, F_2,
        F_3, F_4, F_5, F_6, F_7, F_8, F_9, F10, NML, SCL, HME, KUP, PUP, MIN, LFT,
        CNT, RGT, PLS, END, DWN, PGD, INS, DEL, INV, INV, INV, F11, F12, INV, INV,
        INV, INV, INV, INV, INV, SES, SES, INV, INV, INV, INV, INV, INV, INV, INV, 
        INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, 
        INV, INV, INV, INV, INV, INV, INV,
    },
};

char decode_char(enum KEYCODE scancode)
{
    // tty_printstr("Scancode: ");
    // tty_printhex(scancode);
    // tty_putchar(' ');
    char key = ps2_layout.unshift[scancode];
    // tty_printhex(scancode);
    // tty_putchar(' ');
    // tty_printhex(key);
    // tty_putchar(' ');
    tty_putchar(key);
    // if ((key >= 'a' && key <= 'z') || (key <= '1' && key >= '9')) {
    //     tty_printhex(scancode);
    //     tty_putchar(' ');
    //     tty_printhex(key);
    //     tty_putchar(' ');
    //     tty_putchar(key);
    // }

    // tty_putchar('\n');

    return key;
}

static void key_press(registers_t reg) {
    outb(0x20, 0x20);   // Send EOI

    // tty_printstr("Key pressed\n");
    // tty_printstr("KB INT NO: ");
    // tty_printint(reg.int_no);
    // tty_putchar('\n');

    uint8_t scancode = inb(0x60);
    if (scancode & 0x80) {
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
    // outb(0x64, 0xF0);
    // outb(0x64, 1);
}
