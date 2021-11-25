#include <string.h>

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
