#include "pat/pat.h"
#include "pat_thread.h"
#include <stdlib.h>

int pat_play_thread(void *data) {
    PATPlayThreadData* pat_thread_data = (PATPlayThreadData*) data;

    PATError status = pat_play(pat_thread_data->pat, pat_thread_data->pat_audio_path);

    if(pat_thread_data->callback != NULL) {
        pat_thread_data->callback(status, pat_thread_data->data);
    }

    free(pat_thread_data);

    return 0;
}

int pat_skip_thread(void* data) {
    PATThreadData* pat_thread_data = (PATThreadData*) data;

    PATError status = pat_skip(pat_thread_data->pat);

    if(pat_thread_data->callback != NULL) {
        pat_thread_data->callback(status, pat_thread_data->data);
    }

    free(pat_thread_data);

    return 0;
}

int pat_pause_thread(void* data) {
    PATThreadData* pat_thread_data = (PATThreadData*) data;

    PATError status = pat_pause(pat_thread_data->pat);

    if(pat_thread_data->callback != NULL) {
        pat_thread_data->callback(status, pat_thread_data->data);
    }

    free(pat_thread_data);

    return 0;
}

int pat_resume_thread(void* data) {
    PATThreadData* pat_thread_data = (PATThreadData*) data;

    PATError status = pat_resume(pat_thread_data->pat);

    if(pat_thread_data->callback != NULL) {
        pat_thread_data->callback(status, pat_thread_data->data);
    }

    free(pat_thread_data);

    return 0;
}
