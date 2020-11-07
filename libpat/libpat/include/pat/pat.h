#ifndef PAT_H
#define PAT_H

#include "pat/pat_error.h"

typedef void (*pat_finished_finished_cb_t)(const PATError status, void* data);

PATError pat_init();

PATError pat_play(const char* pat_audio_path);

PATError pat_skip();

PATError pat_pause();

PATError pat_resume();

#endif
