#pragma once

#include <stddef.h>


void kmalloc_init(void);

void *kmalloc(size_t size);

void kfree(void *ptr);
