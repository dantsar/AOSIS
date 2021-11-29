#ifndef _PIC_H
#define _PIC_H

#include <stdbool.h>
#include <stdint.h>

// initalize the PIT timer
void init_timer(uint32_t frequency, bool print_tick);

#endif
