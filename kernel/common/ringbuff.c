#include <stdbool.h>

#include <kernel/common/ringbuff.h>

// read mulitple bytes from the ring buffer
void ring_read(ringbuff_t *rbuff, char *buff, size_t len)
{
    bool empty = false;
    for (size_t i = 0; i < len; i++) {
        char byte = ring_read_byte(rbuff, &empty);
        if (empty == true) {
            break;
        }
        buff[i] = byte;
    }
}

// write a buffer into the ring buffer
void ring_write(ringbuff_t *rbuff, char *buff, size_t len) 
{
    for (size_t i = 0; i < len; i++) {
        ring_write_byte(rbuff, buff[i]);
    }
}

// put a single byte into the buffer
void ring_write_byte(ringbuff_t *rbuff, char elem)
{
    rbuff->buff[rbuff->tail] = elem;
    rbuff->len = (rbuff->len + 1) % rbuff->size;
    rbuff->tail = (rbuff->tail + 1) % rbuff->size;
}

char ring_read_byte(ringbuff_t *rbuff, bool *empty) 
{
    if (rbuff->len == 0 && rbuff->head == rbuff->tail) {
        *empty = true;
        return 0;
    }

    char ret = rbuff->buff[rbuff->head];
    rbuff->head = (rbuff->head + 1) % rbuff->size;
    rbuff->len -= (rbuff->len == 0) ? 0 : 1;
    *empty = false;

    return ret;
}
