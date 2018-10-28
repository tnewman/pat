#include "pat_audio_device.h"
#include "pat_decode.h"
#include <stdlib.h>

typedef struct PAT {
    PATAudioDevice* pat_audio_device;
} PAT;


typedef struct PAT PAT;

PAT* pat_open() {
    PATAudioDevice* pat_audio_device = pat_open_audio_device();

    if(pat_audio_device == NULL) {
        return NULL;
    }

    PAT* pat = malloc(sizeof(PAT));

    if(pat == NULL) {
        pat_free_audio_device(pat_audio_device);
        return NULL;
    }

    pat->pat_audio_device = pat_audio_device;
    return pat;
}

void pat_play(PAT* pat, const char* pat_audio_path) {
    PATDecoder* pat_decoder = pat_open_audio_decoder(pat->pat_audio_device, pat_audio_path);

    if(pat_decoder == NULL) {
        return;
    }

    pat_decode_audio(pat->pat_audio_device, pat_decoder);

    pat_free_audio_decoder(pat_decoder);
}

void pat_close(PAT* pat) {
    if(pat == NULL) {
        return;
    }

    if(pat->pat_audio_device != NULL) {
        pat_free_audio_device(pat->pat_audio_device);
    }

    free(pat);
}
