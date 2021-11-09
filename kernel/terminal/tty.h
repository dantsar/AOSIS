#ifndef _TTY_H
#define _TTY_H

void initialize_terminal();

void terminal_scroll();

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

void terminal_putchar(char c);

void terminal_printstr(const char *str);

void terminal_printstr(const char *str);

#endif
