#include <stdlib.h>
#include <string.h>
#include "pat_ring_buffer.h"

PATRingBuffer* pat_create_ring_buffer(size_t capacity) {
    if(capacity < 1) {
        return NULL;
    }

    PATRingBuffer* ring_buffer = malloc(sizeof(PATRingBuffer));

    if(ring_buffer == NULL) {
        return NULL;
    }

    uint32_t* data = calloc(capacity, sizeof(uint32_t));

    if(data == NULL) {
        free(ring_buffer);
        return NULL;
    }

    ring_buffer->data = data;
    ring_buffer->read_position = 0;
    ring_buffer->write_position = 0;
    ring_buffer->size = 0;
    ring_buffer->capacity = capacity;

    return ring_buffer;
}

size_t pat_read_ring_buffer(PATRingBuffer *ring_buffer, uint32_t *data, size_t data_size) {
    if(data_size < 1) {
        return 0;
    }

    size_t read_size = data_size < ring_buffer->size ? data_size : ring_buffer->size;

    if(read_size <= ring_buffer->capacity - ring_buffer->read_position) {
        memcpy(data, ring_buffer->data + ring_buffer->read_position, read_size);
        ring_buffer->read_position += read_size;

        if(ring_buffer->read_position == ring_buffer->capacity) {
            ring_buffer->read_position = 0;
        }
    } else {
        size_t before_wrap_size = ring_buffer->capacity - ring_buffer->read_position;
        size_t after_wrap_size = read_size - before_wrap_size;
        memcpy(data, ring_buffer->data + ring_buffer->read_position, before_wrap_size);
        memcpy(data + before_wrap_size, ring_buffer->data, after_wrap_size);
        ring_buffer->read_position = after_wrap_size;
    }

    ring_buffer->size -= read_size;

    return read_size;
}

size_t pat_write_ring_buffer(PATRingBuffer *ring_buffer, uint32_t *data, size_t data_size) {
    if(data_size < 1) {
        return 0;
    }

    size_t remaining_capacity = ring_buffer->capacity - ring_buffer->size;
    size_t write_size = data_size < remaining_capacity ? data_size : remaining_capacity;

    if(write_size <= ring_buffer->capacity - ring_buffer->write_position) {
        memcpy(ring_buffer->data + ring_buffer->write_position, data, write_size);
        ring_buffer->write_position += write_size;

        if(ring_buffer->write_position == ring_buffer->capacity) {
            ring_buffer->write_position = 0;
        }
    } else {
        size_t before_wrap_size = ring_buffer->capacity - ring_buffer->write_position;
        size_t after_wrap_size = write_size - before_wrap_size;
        memcpy(ring_buffer->data + ring_buffer->write_position, data, before_wrap_size);
        memcpy(ring_buffer->data, data + before_wrap_size, after_wrap_size);
        ring_buffer->write_position = after_wrap_size;
    }

    ring_buffer->size += write_size;

    return write_size;
}

void pat_free_ring_buffer(PATRingBuffer *ring_buffer) {
    if(ring_buffer != NULL) {
        free(ring_buffer->data);
        free(ring_buffer);
    }
}
