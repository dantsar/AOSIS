#include <stdio.h>

extern void tty_putc(int c);

int putchar(int c)
{
    tty_putc(c);
    
    return 0;
}
