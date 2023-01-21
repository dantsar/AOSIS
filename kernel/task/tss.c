#include <stdint.h>

#include <task/tss.h>

uint8_t __attribute__((aligned (4096))) tss_region[1024];

void tss_init()
{

}
