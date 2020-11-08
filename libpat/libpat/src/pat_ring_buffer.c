#include "pat_ring_buffer.h"
#include <SDL.h>
#include <stdlib.h>
#include <string.h>


typedef struct PATRingBuffer {
    uint8_t *data;
    size_t read_position;
    size_t write_position;
    size_t size;
    size_t capacity;
    SDL_mutex *lock;
    SDL_cond *condition;
} PATRingBuffer;

PATRingBuffer* pat_create_ring_buffer(size_t capacity) {
    if(capacity < 1) {
        return NULL;
    }

    PATRingBuffer* ring_buffer = malloc(sizeof(PATRingBuffer));
    uint8_t* data = calloc(capacity, sizeof(uint8_t));
    SDL_mutex* lock = SDL_CreateMutex();
    SDL_cond* condition = SDL_CreateCond();

    if(ring_buffer == NULL || data == NULL || lock == NULL || condition == NULL) {
        if(data != NULL) {
            free(data);
        }

        if(lock != NULL) {
            SDL_DestroyMutex(lock);
        }

        if(condition != NULL) {
            SDL_DestroyCond(condition);
        }

        if(ring_buffer != NULL) {
            free(ring_buffer);
        }

        return NULL;
    }

    ring_buffer->data = data;
    ring_buffer->lock = lock;
    ring_buffer->condition = condition;
    ring_buffer->read_position = 0;
    ring_buffer->write_position = 0;
    ring_buffer->size = 0;
    ring_buffer->capacity = capacity;

    return ring_buffer;
}

size_t pat_read_ring_buffer(PATRingBuffer *ring_buffer, uint8_t *data, size_t data_size, uint32_t timeout) {
    if(data_size < 1) {
        return 0;
    }

    if(SDL_LockMutex(ring_buffer->lock) != 0) {
        return 0;
    }

    size_t read_size = data_size < ring_buffer->size ? data_size : ring_buffer->size;

    if(read_size < 1) {
        SDL_CondWaitTimeout(ring_buffer->condition, ring_buffer->lock, timeout);
        SDL_UnlockMutex(ring_buffer->lock);
        return 0;
    }

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

    SDL_CondSignal(ring_buffer->condition);
    SDL_UnlockMutex(ring_buffer->lock);

    return read_size;
}

size_t pat_write_ring_buffer(PATRingBuffer *ring_buffer, uint8_t *data, size_t data_size, uint32_t timeout) {
    if(data_size < 1) {
        return 0;
    }

    if(SDL_LockMutex(ring_buffer->lock) != 0) {
        return 0;
    }

    size_t remaining_capacity = ring_buffer->capacity - ring_buffer->size;

    if(remaining_capacity < data_size) {
        SDL_CondWaitTimeout(ring_buffer->condition, ring_buffer->lock, timeout);
        SDL_UnlockMutex(ring_buffer->lock);
        return 0;
    }

    if(data_size <= ring_buffer->capacity - ring_buffer->write_position) {
        memcpy(ring_buffer->data + ring_buffer->write_position, data, data_size);
        ring_buffer->write_position += data_size;

        if(ring_buffer->write_position == ring_buffer->capacity) {
            ring_buffer->write_position = 0;
        }
    } else {
        size_t before_wrap_size = ring_buffer->capacity - ring_buffer->write_position;
        size_t after_wrap_size = data_size - before_wrap_size;
        memcpy(ring_buffer->data + ring_buffer->write_position, data, before_wrap_size);
        memcpy(ring_buffer->data, data + before_wrap_size, after_wrap_size);
        ring_buffer->write_position = after_wrap_size;
    }

    ring_buffer->size += data_size;

    SDL_CondSignal(ring_buffer->condition);
    SDL_UnlockMutex(ring_buffer->lock);

    return data_size;
}

bool pat_clear_ring_buffer(PATRingBuffer *ring_buffer) {
    if(SDL_LockMutex(ring_buffer->lock) != 0) {
        return false;
    }

    memset(ring_buffer->data, 0, ring_buffer->capacity);

    SDL_CondSignal(ring_buffer->condition);
    SDL_UnlockMutex(ring_buffer->lock);

    return true;
}

void pat_free_ring_buffer(PATRingBuffer *ring_buffer) {
    if(ring_buffer != NULL) {
        free(ring_buffer->data);
        SDL_DestroyMutex(ring_buffer->lock);
        SDL_DestroyCond(ring_buffer->condition);
        free(ring_buffer);
    }
}
