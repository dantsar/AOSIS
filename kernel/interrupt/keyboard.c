#include <stddef.h>
#include <stdint.h>

#include <kernel/interrupt/keyboard.h>
#include <kernel/interrupt/interrupt.h>
#include <kernel/terminal/tty.h>

unsigned char last_char = 0;

static void key_press(struct registers reg) {
    last_char = 'a';
    outb(0x20, 0x20);   // Send EOI
    tty_printstr("Key pressed\n");
    tty_printstr("KB INT NO: ");
    tty_printint(reg.int_no);
    tty_putchar('\n');
    unsigned char scancode = inb(0x60);
    if (scancode & 0x80) {
        tty_printstr("RELEASED\n");
        last_char = 'a';
    } else {
        tty_printstr("PRESSED\n");
    }
}

void mouse(){
    tty_printstr("Mouse\n");
}

void init_keyboard() 
{
    idt_handlers[33] = (idt_handler)&key_press;
    idt_handlers[31+12] = (idt_handler)&mouse;

}
