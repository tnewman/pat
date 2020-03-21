#include "pat/pat.h"
#include "pat/pat_error.h"
#include "pat_audio_device.h"
#include "pat_decode.h"
#include <stdlib.h>

typedef struct PAT {
    PATAudioDevice* pat_audio_device;
} PAT;

PATError pat_open(PAT** pat_out) {
    *pat_out = NULL;
    PATError status;

    // Required for older versions of FFmpeg, such as the one bundled with 
    // Ubuntu 18.04.
    av_register_all();

    pat_init_audio_decoder();

    PATAudioDevice* pat_audio_device;

    if((status = pat_open_audio_device(&pat_audio_device)) != PAT_SUCCESS) {
        return status;
    }

    PAT* pat = malloc(sizeof(PAT));

    if(pat == NULL) {
        pat_free_audio_device(pat_audio_device);
        return PAT_MEMORY_ERROR;
    }

    pat->pat_audio_device = pat_audio_device;
    *pat_out = pat;

    return status;
}

PATError pat_play(const PAT* pat, const char* pat_audio_path) {
    pat_resume(pat);
    return pat_decode_audio(pat->pat_audio_device, pat_audio_path);
}

PATError pat_skip(const PAT* pat) {
    pat_resume(pat);
    return pat_skip_audio(pat->pat_audio_device);
}

PATError pat_pause(const PAT* pat) {
    return pat_pause_audio_device(pat->pat_audio_device);
}

PATError pat_resume(const PAT* pat) {
    return pat_resume_audio_device(pat->pat_audio_device);
}

void pat_close(PAT* pat) {
    if(pat == NULL) {
        return;
    }

    pat_skip(pat);

    if(pat->pat_audio_device != NULL) {
        pat_free_audio_device(pat->pat_audio_device);
    }

    free(pat);
}
