#pragma once

#include <stdbool.h>
#include <stdint.h>

// initalize the PIT timer
void pic_init(uint32_t frequency, bool print_tick);

uint32_t pic_get_tick(void);
