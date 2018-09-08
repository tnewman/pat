#include "pat_decode.h"
#include "SDL.h"
#include <stdlib.h>

typedef struct PATAudioDevice {
    uint32_t device_id;
    int32_t frequency;
    uint8_t channels;
    uint16_t samples;
} PATAudioDevice;

void pat_audio_callback(void* userdata, Uint8* stream, int len);

PATAudioDevice* pat_open_audio_device() {
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec want;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = 48000;
    want.format = AUDIO_S32SYS;
    want.channels = 6;
    want.samples = 4096;
    want.callback = pat_audio_callback;

    SDL_AudioSpec have;

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if(device_id < 0) {
        return NULL;
    }

    PATAudioDevice* pat_audio_device = malloc(sizeof(PATAudioDevice));

    if(pat_audio_device == NULL) {
        return NULL;
    }

    pat_audio_device->device_id = device_id;
    pat_audio_device->frequency = have.freq;
    pat_audio_device->channels = have.channels;
    pat_audio_device->samples = have.samples;

    SDL_PauseAudioDevice(device_id, 0);

    return pat_audio_device;
}

void pat_audio_callback(void* userdata, Uint8* stream, int len) {

}

void pat_free_audio_device(PATAudioDevice* pat_audio_device) {
    SDL_CloseAudio();

    if(pat_audio_device != NULL) {
        free(pat_audio_device);
    }
}
