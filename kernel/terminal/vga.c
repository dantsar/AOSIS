#include <stddef.h>
#include <stdint.h>

#include <kernel/terminal/vga.h>

/* every vga color entry is two nibbles 0: foreground color, 1: background color */
inline uint8_t vga_color_entry(enum vga_color fg, enum vga_color bg) 
{
	return fg | (bg << 4);
}

inline uint16_t vga_entry(uint8_t uc, uint8_t color) 
{
	return (uint16_t) uc | ((uint16_t) color << 8);
}
