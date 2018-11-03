#ifndef PAT_ERROR_H
#define PAT_ERROR_H

typedef enum PATError {
    PAT_SUCCESS = 0,
    PAT_AUDIO_DEVICE_ERROR = 1,
    PAT_DEMUX_ERROR = 2,
    PAT_DECODE_ERROR = 3,
    PAT_FILE_OPEN_ERROR = 4,
    PAT_INTERRUPTED_ERROR = 5,
    PAT_MEMORY_ERROR = 6,
    PAT_RESAMPLE_ERROR = 7,
    PAT_TERMINATED_ERROR = 8,
    PAT_UNKNOWN_ERROR = 9
} PATError;

char* pat_error_to_string(PATError error);

#endif
