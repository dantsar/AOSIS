#ifndef _TTY_H
#define _TTY_H

#include <stddef.h>
#include <stdint.h>

void initialize_terminal();

void update_cursor(int x, int y);

void tty_clear();

void tty_scroll();

void tty_putentryat(char c, uint8_t color, size_t x, size_t y);

void tty_putc(char c);

void tty_deleteprev();

void tty_write(const char *str, size_t len);

void tty_printstr(const char *str);

// print out num in hex 
void tty_printhex(uint32_t num);

void tty_printint(uint32_t num);

int kprintf(const char *format, ...);

#endif
