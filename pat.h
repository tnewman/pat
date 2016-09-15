int pat_init(void);
int pat_quit(void);
int pat_play(const char* audio_file_path);

enum pat_error {
    PAT_SUCCESS,
    PAT_NOT_INITIALIZED_ERROR,
    PAT_INIT_ERROR,
    PAT_OPEN_DEVICE_ERROR,
    PAT_OPEN_FILE_ERROR,
    PAT_NO_AUDIO_STREAM_ERROR,
    PAT_CODEC_ERROR,
    PAT_RESAMPLE_ERROR,
    PAT_PLAYBACK_ERROR
};
