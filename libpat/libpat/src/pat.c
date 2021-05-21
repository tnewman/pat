#include "pat/pat.h"
#include "pat/pat_error.h"
#include "pat_audio_device.h"
#include "pat_decode.h"
#include <libavformat/version.h>
#include <stdlib.h>

static void pat_close();

PATAudioDevice* pat_audio_device = NULL;

#define FFMPEG_4_AVFORMAT_VERSION 58

PATError pat_init() {
    PATError status;

    // Required for older versions of FFmpeg, such as the one bundled with 
    // Ubuntu 18.04.
    #if LIBAVFORMAT_VERSION_MAJOR < FFMPEG_4_AVFORMAT_VERSION
    av_register_all();
    #endif

    pat_init_audio_decoder();

    if((status = pat_open_audio_device(&pat_audio_device)) != PAT_SUCCESS) {
        return status;
    }

    atexit(pat_close);

    return status;
}

PATError pat_play(const char* pat_audio_path) {
    pat_resume();
    return pat_decode_audio(pat_audio_device, pat_audio_path);
}

PATError pat_skip() {
    pat_resume();
    return pat_skip_audio(pat_audio_device);
}

PATError pat_pause() {
    return pat_pause_audio_device(pat_audio_device);
}

PATError pat_resume() {
    return pat_resume_audio_device(pat_audio_device);
}

static void pat_close() {
    pat_skip();

    if(pat_audio_device != NULL) {
        pat_free_audio_device(pat_audio_device);
    }
}
