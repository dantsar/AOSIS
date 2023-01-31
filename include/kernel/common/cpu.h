#pragma once

// clear interrupt flag
void cli();

void sti();

// halt the cpu until an interrupt occurs
void halt();
