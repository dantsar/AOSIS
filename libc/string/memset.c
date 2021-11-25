#include <string.h>

void *memset(void *s, int c, size_t n)
{
	unsigned char *sptr = (unsigned char *) s;
	for (size_t i = 0; i < n; i++) {
		sptr[i] = c;
	}
	return sptr;
}
