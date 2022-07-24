#ifndef _BITMAP_H
#define _BITMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct bitmap {
    size_t size;
    uint8_t *index;
};

typedef struct bitmap bitmap_t;

void bitmap_set(bitmap_t *bmap, size_t index, bool val);

bool bitmap_get(bitmap_t *bmap, size_t index);

#endif
