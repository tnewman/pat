typedef struct PAT PAT;

PAT* pat_open();

void pat_play(PAT* pat, char* pat_audio_path);

void pat_close(PAT* pat);
