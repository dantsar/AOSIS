#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>
#include <stddef.h>

size_t strlen(const char *str);

size_t strnlen(const char *str, size_t maxlen);

int memcmp(const void *s1, const void *s2, size_t n);

void *memcpy(void *dest, const void *src, size_t n);

void *memmove(void *dest, const void *src, size_t n);

void *memset(void *s, int c, size_t n);

#endif
