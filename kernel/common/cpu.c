#include <common/cpu.h>

void cli()
{
    asm volatile("cli");
}

void halt() 
{
    asm volatile("hlt");
    return;
}
