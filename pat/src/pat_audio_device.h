#ifndef PAT_AUDIO_DEVICE_H
#define PAT_AUDIO_DEVICE_H

#include "pat_ring_buffer.h"
#include <stdint.h>

typedef struct PATAudioDevice {
    uint32_t device_id;
    int32_t frequency;
    uint16_t format;
    uint8_t channels;
    uint16_t samples;
    PATRingBuffer* pat_ring_buffer;
} PATAudioDevice;

PATAudioDevice* pat_open_audio_device();

void pat_free_audio_device(PATAudioDevice* pat_audio_device);

#endif
