#include <common/cpu.h>

void cli()
{
    asm volatile("cli");
}

void sti()
{
    asm volatile("sti");
}

void halt()
{
    asm volatile("hlt");
    return;
}
