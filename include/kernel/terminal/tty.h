#ifndef _TTY_H
#define _TTY_H

void initialize_terminal();

void tty_clear();

void tty_scroll();

void tty_putentryat(char c, uint8_t color, size_t x, size_t y);

void tty_putchar(char c);

void tty_printstr(const char *str);

void tty_printstr(const char *str);

// print out num in hex 
void tty_printint(uint32_t num);

#endif
