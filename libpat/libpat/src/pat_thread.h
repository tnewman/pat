#ifndef PAT_THREAD_H
#define PAT_THREAD_H

typedef struct PATThreadData {
    const PAT* pat;
    pat_finished_finished_cb_t callback;
    void* data;
} PATThreadData;

typedef struct PATPlayThreadData {
    const PAT* pat;
    const char* pat_audio_path;
    pat_finished_finished_cb_t callback;
    void* data;
} PATPlayThreadData;

int pat_play_thread(void* data);

int pat_skip_thread(void* data);

int pat_pause_thread(void* data);

int pat_resume_thread(void* data);

#endif
