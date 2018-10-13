#ifndef PAT_RING_BUFFER_H
#define PAT_RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>

typedef struct PATRingBuffer PATRingBuffer;

PATRingBuffer* pat_create_ring_buffer(size_t capacity);

size_t pat_read_ring_buffer(PATRingBuffer *ring_buffer, uint8_t *data, size_t data_size, uint32_t timeout);

size_t pat_write_ring_buffer(PATRingBuffer *ring_buffer, uint8_t *data, size_t data_size, uint32_t timeout);

size_t pat_get_ring_buffer_size(PATRingBuffer* ring_buffer);

void pat_free_ring_buffer(PATRingBuffer *ring_buffer);

#endif
