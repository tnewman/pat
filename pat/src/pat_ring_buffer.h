#include <stddef.h>
#include <stdint.h>

typedef struct PATRingBuffer {
    uint32_t* data;
    size_t read_position;
    size_t write_position;
    size_t size;
    size_t capacity;
} PATRingBuffer;

PATRingBuffer* pat_ring_buffer_create(size_t capacity);

size_t pat_ring_buffer_read(PATRingBuffer *ring_buffer, uint32_t *data, size_t data_size);

size_t pat_ring_buffer_write(PATRingBuffer *ring_buffer, uint32_t *data, size_t data_size);

void pat_ring_buffer_free(PATRingBuffer *ring_buffer);
