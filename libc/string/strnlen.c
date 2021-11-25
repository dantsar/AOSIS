#include <string.h>

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
