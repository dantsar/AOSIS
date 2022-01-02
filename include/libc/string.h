#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

size_t strlen(const char *str);

size_t strnlen(const char *str, size_t maxlen);

int strcmp(const char *s1, const char *s2);

int memcmp(const void *s1, const void *s2, size_t n);

void *memcpy(void *dest, const void *src, size_t n);

void *memmove(void *dest, const void *src, size_t n);

void *memset(void *s, int c, size_t n);

#endif
