#include <stddef.h>
#include <stdint.h>

#include "interrupt/interrupt.h"
#include "terminal/tty.h"
#include "terminal/vga.h"

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

void kmain() 
{
	init_interrupt();
	initialize_terminal();

	tty_printstr("UGH\n");
	init_timer(20);
	
	asm volatile ("int $0x0");
	asm volatile ("int $0x1");
	asm volatile ("int $0x2");
	asm volatile ("int $0x0");
	asm volatile ("int $0x1");
	asm volatile ("int $0x2");

	tty_printstr("UH OH\n");

}
