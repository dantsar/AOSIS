#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *destptr = (unsigned char *) dest;
	const unsigned char *srcptr = (const unsigned char *) src;
	for (size_t i = 0; i < n; i++) {
		destptr[i] = srcptr[i];
	}
	return dest;
}
