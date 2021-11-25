#include <string.h>

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
