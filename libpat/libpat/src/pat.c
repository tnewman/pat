#include "pat/pat.h"
#include "pat/pat_error.h"
#include "pat_audio_device.h"
#include "pat_decode.h"
#include "pat_thread.h"
#include <stdlib.h>

typedef struct PAT {
    PATAudioDevice* pat_audio_device;
} PAT;

static PATError pat_run_async_thread(const PAT* pat, SDL_ThreadFunction thread_function, const char* thread_name,
                                pat_finished_finished_cb_t callback, void* data);

PATError pat_open(PAT** pat_out) {
    *pat_out = NULL;
    PATError status;

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

PATError pat_play_async(const PAT* pat, const char* pat_audio_path, pat_finished_finished_cb_t callback, void* data) {
    PATPlayThreadData* pat_thread_data = malloc(sizeof(PATPlayThreadData));

    if(pat_thread_data == NULL) {
        return PAT_MEMORY_ERROR;
    }

    pat_thread_data->pat = pat;
    pat_thread_data->pat_audio_path = pat_audio_path;
    pat_thread_data->callback = callback;
    pat_thread_data->data = data;

    SDL_Thread* thread = SDL_CreateThread(pat_play_thread, "PAT Play Thread", (void*) pat_thread_data);

    if(thread == NULL) {
        free(pat_thread_data);
        return PAT_MEMORY_ERROR;
    }

    SDL_DetachThread(thread);

    return PAT_SUCCESS;
}

PATError pat_skip(const PAT* pat) {
    pat_resume(pat);
    return pat_skip_audio(pat->pat_audio_device);
}

PATError pat_skip_async(const PAT* pat, pat_finished_finished_cb_t callback, void* data) {
    return pat_run_async_thread(pat, pat_skip_thread, "PAT Skip Thread", callback, data);
}

PATError pat_pause(const PAT* pat) {
    return pat_pause_audio_device(pat->pat_audio_device);
}

PATError pat_pause_async(const PAT* pat, pat_finished_finished_cb_t callback, void* data) {
    return pat_run_async_thread(pat, pat_pause_thread, "PAT Pause Thread", callback, data);
}

PATError pat_resume(const PAT* pat) {
    return pat_resume_audio_device(pat->pat_audio_device);
}

PATError pat_resume_async(const PAT* pat, pat_finished_finished_cb_t callback, void* data) {
    return pat_run_async_thread(pat, pat_resume_thread, "PAT Resume Thread", callback, data);
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

static PATError pat_run_async_thread(const PAT* pat, SDL_ThreadFunction thread_function, const char* thread_name,
                                pat_finished_finished_cb_t callback, void* data) {
    PATThreadData* pat_thread_data = malloc(sizeof(PATThreadData));

    if(pat_thread_data == NULL) {
        return PAT_MEMORY_ERROR;
    }

    pat_thread_data->pat = pat;
    pat_thread_data->callback = callback;
    pat_thread_data->data = data;

    SDL_Thread* thread = SDL_CreateThread(thread_function, thread_name, (void*) pat_thread_data);

    if(thread == NULL) {
        free(pat_thread_data);
        return PAT_MEMORY_ERROR;
    }

    SDL_DetachThread(thread);

    return PAT_SUCCESS;
}
