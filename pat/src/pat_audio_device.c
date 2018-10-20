#include "pat_decode.h"
#include "SDL.h"
#include <stdlib.h>

void pat_audio_callback(void* userdata, Uint8* stream, int len);

PATAudioDevice* pat_open_audio_device() {
    PATRingBuffer* pat_ring_buffer = pat_create_ring_buffer(32768);

    if(pat_ring_buffer == NULL) {
        return NULL;
    }

    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec want;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = 48000;
    want.format = AUDIO_S16SYS;
    want.channels = 6;
    want.samples = 1024;
    want.silence = 0;
    want.callback = pat_audio_callback;
    want.userdata = pat_ring_buffer;

    SDL_AudioSpec have;

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if(device_id < 0) {
        pat_free_ring_buffer(pat_ring_buffer);
        return NULL;
    }

    PATAudioDevice* pat_audio_device = malloc(sizeof(PATAudioDevice));

    if(pat_audio_device == NULL) {
        pat_free_ring_buffer(pat_ring_buffer);
        return NULL;
    }

    pat_audio_device->device_id = device_id;
    pat_audio_device->frequency = have.freq;
    pat_audio_device->format = have.format;
    pat_audio_device->channels = have.channels;
    pat_audio_device->samples = have.samples;
    pat_audio_device->pat_ring_buffer = pat_ring_buffer;

    SDL_PauseAudioDevice(device_id, 0);

    return pat_audio_device;
}

void pat_audio_callback(void* userdata, Uint8* stream, int len) {
    PATRingBuffer* pat_ring_buffer = (PATRingBuffer*) userdata;
    pat_read_ring_buffer(pat_ring_buffer, stream, (size_t) len, 100);
}

void pat_free_audio_device(PATAudioDevice* pat_audio_device) {
    SDL_Quit();

    if(pat_audio_device == NULL) {
        return;
    }

    SDL_CloseAudioDevice(pat_audio_device->device_id);

    if(pat_audio_device->pat_ring_buffer != NULL) {
        pat_free_ring_buffer(pat_audio_device->pat_ring_buffer);
    }

    free(pat_audio_device);
}
