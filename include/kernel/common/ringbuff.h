#ifndef _RINGBUFF_H
#define _RINGBUFF_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// lossfull ring buffer implementation: old data is overwritten
// if not consumed fast enough

struct ring_buffer {
    size_t size;
    char *buff;
    size_t len;    // number of elements in buff (capped at size)
    size_t head, tail;  // head and tail just keep incrementing
};

typedef struct ring_buffer ringbuff_t;

// write a byte to the ring buffer
void ring_write_byte(ringbuff_t *rbuff, char elem);

// read a byte from the ring buffer
char ring_read_byte(ringbuff_t *rbuff, bool *empty);

// read multiple bytes into the ring buffer
void ring_read(ringbuff_t *rbuff, char *buff, size_t len);

// write multiple bytes into the ring buffer
void ring_write(ringbuff_t *rbuff, char *buff, size_t len);

#endif
