#include "pat_audio_device.h"
#include "pat_decode.h"
#include <stdlib.h>

static const int SAMPLES_PER_SECOND = 48000;

static const int AUDIO_CHANNELS = 6; // 6 means 5.1 channel sound

static const int AUDIO_BUFFER_SIZE = 4096; // SDL requires a power of 2 sized buffer. This value should not be too
                                           // low, or audio clipping will result. This value should not be too high,
                                           // or initial lag will occur.
static const int AUDIO_RING_BUFFER_SIZE = 131072; // Must be large enough to keep up with SDL's demand for audio data.
                                                  // Choppy playback will occur if this value is too low. The ring
                                                  // buffer size will need to increase as the audio buffer size
                                                  // increases.

static void pat_audio_callback(void* userdata, Uint8* stream, int len);

PATError pat_open_audio_device(PATAudioDevice** pat_audio_device_out) {
    PATRingBuffer* pat_ring_buffer = pat_create_ring_buffer(AUDIO_RING_BUFFER_SIZE);

    if(pat_ring_buffer == NULL) {
        *pat_audio_device_out = NULL;
        return PAT_MEMORY_ERROR;
    }

    #ifdef _WIN32
        // SDL2 default audio driver is broken on Windows
        _putenv("SDL_AUDIODRIVER=winmm");
    #endif

    if(SDL_Init(SDL_INIT_AUDIO) != 0) {
        return PAT_AUDIO_DEVICE_ERROR;
    }

    SDL_AudioSpec want;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = SAMPLES_PER_SECOND;
    want.format = AUDIO_S16SYS;
    want.channels = AUDIO_CHANNELS;
    want.samples = AUDIO_BUFFER_SIZE;
    want.silence = 0;
    want.callback = pat_audio_callback;
    want.userdata = pat_ring_buffer;

    SDL_AudioSpec have;

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if(device_id <= 0) {
        pat_free_ring_buffer(pat_ring_buffer);
        *pat_audio_device_out = NULL;
        return PAT_AUDIO_DEVICE_ERROR;
    }

    SDL_mutex* audio_lock = SDL_CreateMutex();

    if(audio_lock == NULL) {
        pat_free_ring_buffer(pat_ring_buffer);
        SDL_CloseAudioDevice(device_id);
        *pat_audio_device_out = NULL;
        return PAT_MEMORY_ERROR;
    }

    PATAudioDevice* pat_audio_device = malloc(sizeof(PATAudioDevice));

    if(pat_audio_device == NULL) {
        pat_free_ring_buffer(pat_ring_buffer);
        SDL_CloseAudioDevice(device_id);
        SDL_DestroyMutex(audio_lock);
        *pat_audio_device_out = NULL;
        return PAT_MEMORY_ERROR;
    }

    SDL_AtomicSet(&pat_audio_device->audio_wait_count, 0);

    pat_audio_device->device_id = device_id;
    pat_audio_device->frequency = have.freq;
    pat_audio_device->format = have.format;
    pat_audio_device->channels = have.channels;
    pat_audio_device->pat_ring_buffer = pat_ring_buffer;
    pat_audio_device->audio_lock = audio_lock;

    *pat_audio_device_out = pat_audio_device;

    SDL_PauseAudioDevice(device_id, 0);

    return PAT_SUCCESS;
}

PATError pat_pause_audio_device(const PATAudioDevice* pat_audio_device) {
    SDL_PauseAudioDevice(pat_audio_device->device_id, 1);
    return PAT_SUCCESS;
}

PATError pat_resume_audio_device(const PATAudioDevice* pat_audio_device) {
    SDL_PauseAudioDevice(pat_audio_device->device_id, 0);
    return PAT_SUCCESS;
}

static void pat_audio_callback(void* userdata, Uint8* stream, int len) {
    PATRingBuffer* pat_ring_buffer = (PATRingBuffer*) userdata;
    size_t read_size = pat_read_ring_buffer(pat_ring_buffer, stream, (size_t) len, 500);

    if (read_size < (size_t) len) {
        memset(stream + read_size, 0, (size_t) len - read_size);
    }
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

    if(pat_audio_device->audio_lock != NULL) {
        SDL_DestroyMutex(pat_audio_device->audio_lock);
    }

    free(pat_audio_device);
}
