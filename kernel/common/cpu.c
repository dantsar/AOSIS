#include <common/cpu.h>

void halt() 
{
    asm volatile("hlt");
    return;
}
