#ifndef PAT_AUDIO_DEVICE_H
#define PAT_AUDIO_DEVICE_H

#include "pat/pat_error.h"
#include "pat_ring_buffer.h"
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct PATAudioDevice {
    uint32_t device_id;
    int32_t frequency;
    uint16_t format;
    uint8_t channels;
    PATRingBuffer* pat_ring_buffer;
    SDL_sem* concurrent_plays;
} PATAudioDevice;

PATError pat_open_audio_device(PATAudioDevice** pat_audio_device_out);

void pat_free_audio_device(PATAudioDevice* pat_audio_device);

#endif
