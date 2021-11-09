#include <stdint.h>
#include <stddef.h>

#include "tty.h"
#include "vga.h"

static const size_t SCREEN_WIDTH = 80;
static const size_t SCREEN_HEIGHT = 25;
static size_t terminal_ypos;
static size_t terminal_xpos;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

/* -------------------------------------------------------- */

void initialize_terminal()
{
	terminal_xpos = 0;
	terminal_ypos = 0;
	terminal_buffer = (uint16_t *) 0xB8000;
	terminal_color = vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

	for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (size_t x = 0; x < SCREEN_WIDTH; x++) {
			size_t index = y * SCREEN_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_scroll()
{
	// erase the first row and move the remaining rows up */
	for (size_t y = 0; y < SCREEN_HEIGHT-1; y++) {
		for (size_t x = 0; x < SCREEN_WIDTH; x++) {
			size_t index = y * SCREEN_WIDTH + x;
			size_t copy_index = (y+1) * SCREEN_WIDTH + x;
			terminal_buffer[index] = terminal_buffer[copy_index];
		}
	}

	for (size_t x = 0; x < SCREEN_WIDTH; x++) {
		size_t index = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH + x;
		terminal_buffer[index] = ' ';
	}
	terminal_ypos--;

}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	size_t index = y * SCREEN_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	if (c == '\n') {
		terminal_xpos = 0;
		if (++terminal_ypos == SCREEN_HEIGHT) {
			terminal_scroll();
		}
		return;
	}

	terminal_putentryat(c, terminal_color, terminal_xpos, terminal_ypos);
	if (++terminal_xpos == SCREEN_WIDTH) {
		terminal_xpos = 0;
		if (++terminal_ypos == SCREEN_HEIGHT) { 
			terminal_scroll();
			// terminal_ypos = 0;
		}
	}
}

void terminal_write(const char *str, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		terminal_putchar(str[i]);
	}
}

void terminal_printstr(const char *str) 
{ 
	size_t index = 0;
	while (str[index]) {
		terminal_putchar(str[index]);
		index++;
	}
}
