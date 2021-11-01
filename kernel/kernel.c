#include <stddef.h>
#include <stdint.h>

// strings.h
size_t strlen(const char *str)
{
	size_t len = 0;
	while(str[len]) {
		len++;
	}
	return len;
}

size_t strnlen(const char *str, size_t maxlen)
{
	size_t len = 0;
	for (size_t i = 0; i < maxlen; i++) {
		if (str[i] == '\0') {
			break;
		}
	}
	return len;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
	const char *s1ptr = (const char *) s1;
	const char *s2ptr = (const char *) s2;
	int cmp = 0;
	for (size_t i = 0; i < n; i++) {
		if (s1ptr[i] != s2ptr[i]) {
			cmp = s1ptr[i] - s2ptr[i];
			break;
		}
	}
	return cmp;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *destptr = (unsigned char *) dest;
	const unsigned char *srcptr = (const unsigned char *) src;
	for (size_t i = 0; i < n; i++) {
		destptr[i] = srcptr[i];
	}
	return dest;
}

void *memmove(void *dest, const void *src, size_t n){
	unsigned char *destptr = (unsigned char *) dest;
	const unsigned char *srcptr = (const unsigned char *) src;

	if (dest < src) {
		for (size_t i = 0; i < n; i++) {
			destptr[i] = srcptr[i];
		}
	} else {
		for (size_t i = n; i != 0; i--) {
			destptr[i-1] = srcptr[i-1];
		}
	}
	return destptr;
}

void *memset(void *s, int c, size_t n)
{
	unsigned char *sptr = (unsigned char *) s;
	for (size_t i = 0; i < n; i++) {
		sptr[i] = c;
	}
	return sptr;
}

/* -------------------------------------------------------- */
/* constants, variables, and functions for VGA */

static const size_t SCREEN_WIDTH = 80;
static const size_t SCREEN_HEIGHT = 25;
size_t terminal_ypos;
size_t terminal_xpos;
uint8_t terminal_color;
uint16_t *terminal_buffer;

/* constants for Hardware defined text mode colors */
// vga.h 
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/* every vga color entry is two nibbles 0: foreground color, 1: background color */
static inline uint8_t vga_color_entry(enum vga_color fg, enum vga_color bg) 
{
	return fg | (bg << 4);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | ((uint16_t) color << 8);
}

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

void terminal_scroll() {
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

void kernel_main() 
{
	initialize_terminal();

	size_t i = 0;
	for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
		terminal_putchar('a' + ((i++) % 26));
		terminal_putchar('\n');
	}
	terminal_printstr("LAST LINE");
}
