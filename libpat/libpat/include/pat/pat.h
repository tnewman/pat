#ifndef PAT_H
#define PAT_H

#include "pat/pat_error.h"

typedef struct PAT PAT;

PATError pat_open(PAT** pat_out);

PATError pat_play(const PAT* pat, const char* pat_audio_path);

PATError pat_skip(const PAT* pat);

PATError pat_pause(const PAT* pat);

PATError pat_resume(const PAT* pat);

void pat_close(PAT* pat);

#endif
