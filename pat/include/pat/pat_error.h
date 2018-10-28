#ifndef PAT_ERROR_H
#define PAT_ERROR_H

typedef enum PATError {
    PAT_SUCCESS,
    PAT_AUDIO_DEVICE_ERROR,
    PAT_FILE_OPEN_ERROR,
    PAT_DEMUX_ERROR,
    PAT_DECODE_ERROR,
    PAT_RESAMPLE_ERROR,
    PAT_MEMORY_ERROR,
    PAT_UNKNOWN_ERROR
} PATError;

char* pat_error_to_string(PATError error);

#endif
