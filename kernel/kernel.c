#include <stddef.h>
#include <stdint.h>

#include "interrupt/interrupt.h"
#include "interrupt/pic.h"
#include "interrupt/keyboard.h"
#include "terminal/tty.h"
#include "terminal/vga.h"

// strings.h: TO DO {Create static library}
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


// static inline int are_interrupts_enabled()
// {
//     unsigned long flags;
//     asm volatile ( "pushf\n\t"
//                    "pop %0"
//                    : "=g"(flags) );
//     return flags & (1 << 9);
// }

void kmain() 
{
	initialize_terminal();
	init_interrupt();

    // asm volatile("int $0x00");
	// if (are_interrupts_enabled()) {
	// 	tty_printstr("Interrupts are enabled\n");
	// }
	// for(;;);

	tty_printstr("UGH\n");

    asm volatile("sti");
	// asm volatile ("int $0x0");
	// // asm volatile ("int $0x1");
	// // asm volatile ("int $0x2");
	// // asm volatile ("int $0x3");
	// // asm volatile ("int $0x4");
	// // asm volatile ("int $0x5");
	// // asm volatile ("int $0x6");
	// // asm volatile ("int $0x7");
	// asm volatile ("int $0x8"); // suss
	// asm volatile ("int $0x9");
	// // asm volatile ("int $0xa"); // suss
	// // asm volatile ("int $0xb"); // suss
	// // asm volatile ("int $0xc"); // suss
	// // asm volatile ("int $0xd"); // suss
	// // asm volatile ("int $0xe"); // suss
	// asm volatile ("int $0xf");
	// asm volatile ("int $0x10");
	// // asm volatile ("int $0x11"); // not that suss
	// asm volatile ("int $0x12");
	// asm volatile ("int $0x13");
	// asm volatile ("int $0x14");
	// asm volatile ("int $0x15");
	// asm volatile ("int $0x16");
	// asm volatile ("int $0x17");
	// asm volatile ("int $0x18");
	// asm volatile ("int $0x19");
	// asm volatile ("int $0x1a");
	// asm volatile ("int $0x1b");
	// asm volatile ("int $0x1c");
	// asm volatile ("int $0x1d");
	// asm volatile ("int $0x1e"); // not that suss
	// asm volatile ("int $0x1f");
	
	// init_timer(19);
	// init_keyboard();
	// asm volatile("sti");

	// tty_printstr("Test timer interrupt\n");
	// for (int i = 0; i < 10; i++)
	// 	asm volatile ("int $32");

	// asm volatile ("int $32");


	// tty_printstr("UH OH\n");

	for(;;); // spin forever
}
