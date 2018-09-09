#ifndef PAT_RING_BUFFER_H
#define PAT_RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct PATRingBuffer {
    uint32_t* data;
    size_t read_position;
    size_t write_position;
    size_t size;
    size_t capacity;
} PATRingBuffer;

PATRingBuffer* pat_create_ring_buffer(size_t capacity);

size_t pat_read_ring_buffer(PATRingBuffer *ring_buffer, uint32_t *data, size_t data_size);

size_t pat_write_ring_buffer(PATRingBuffer *ring_buffer, uint32_t *data, size_t data_size);

void pat_free_ring_buffer(PATRingBuffer *ring_buffer);

#endif
