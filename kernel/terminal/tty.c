#include <stdint.h>
#include <stddef.h>

#include <common/ports.h>
#include <terminal/tty.h>
#include <terminal/vga.h>

static const size_t SCREEN_WIDTH = 80;
static const size_t SCREEN_HEIGHT = 25;
static size_t tty_ypos;
static size_t tty_xpos;
static uint8_t tty_color;
static uint16_t *tty_buffer;

void initialize_terminal()
{
	tty_xpos = 0;
	tty_ypos = 0;

	tty_clear();

	// init vga cursor
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | (uint8_t) 0);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | (uint8_t)15);

	update_cursor(0, 0);
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * SCREEN_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void tty_clear() {
	tty_buffer = (uint16_t *) 0xB8000;
	tty_color = vga_color_entry(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

	for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (size_t x = 0; x < SCREEN_WIDTH; x++) {
			size_t index = y * SCREEN_WIDTH + x;
			tty_buffer[index] = vga_entry(' ', tty_color);
		}
	}
	tty_xpos = 0;
	tty_ypos = 0;
}

void tty_scroll()
{
	// erase the first row and move the remaining rows up */
	for (size_t y = 0; y < SCREEN_HEIGHT-1; y++) {
		for (size_t x = 0; x < SCREEN_WIDTH; x++) {
			size_t index = y * SCREEN_WIDTH + x;
			size_t copy_index = (y+1) * SCREEN_WIDTH + x;
			tty_buffer[index] = tty_buffer[copy_index];
		}
	}

	for (size_t x = 0; x < SCREEN_WIDTH; x++) {
		size_t index = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH + x;
		tty_buffer[index] = ' ';
	}
	tty_ypos--;

}

void tty_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	size_t index = y * SCREEN_WIDTH + x;
	tty_buffer[index] = vga_entry(c, color);
}

void tty_putchar(char c)
{
	if (c == '\n') {
		tty_xpos = 0;
		if (++tty_ypos == SCREEN_HEIGHT) {
			tty_scroll();
		}
		return;
	} else if (c == '\t'){
		for (int i = 0; i < 8; i++) {
			tty_putchar(' ');
		}
		return;
	}

	tty_putentryat(c, tty_color, tty_xpos, tty_ypos);
	if (++tty_xpos == SCREEN_WIDTH) {
		tty_xpos = 0;
		if (++tty_ypos == SCREEN_HEIGHT) { 
			tty_scroll();
		}
	}
	update_cursor(tty_xpos, tty_ypos);
}

void tty_write(const char *str, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		tty_putchar(str[i]);
	}
}

void tty_printstr(const char *str) 
{ 
	size_t index = 0;
	while (str[index]) {
		tty_putchar(str[index]);
		index++;
	}
}

void tty_printint(uint32_t num) 
{
	if (num == 0) { 
		tty_putchar('0');
		return;
	}
	
	char s[32] = {0};

	int i;
	for (i = 0; num > 0; i++) {
		s[i] = num % 10;
		num /= 10;
	}
	i--;
	while (i >= 0) {
		tty_putchar('0' + s[i--]);
	}
}

void tty_printhex(uint32_t num)
{
	tty_printstr("0x");
	if (num == 0) {
		tty_putchar('0');
		return;
	}

	static const char hex[] = "0123456789abcdef";

	int c;
	for (size_t i = 0; i < 2*sizeof(num); i++) {
		if (num == 0) {
			return;
		}
		c = ((num & 0xF0000000) >> 28) & 0x0F; // done to avoid reversing string
		tty_putchar(hex[c]);

		num <<= 4;
	}
}
