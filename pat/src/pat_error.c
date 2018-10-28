#include "pat/pat_error.h"

char* pat_error_to_string(const PATError error) {
    switch(error) {
        case PAT_SUCCESS:
            return "Success.";
        case PAT_AUDIO_DEVICE_ERROR:
            return "Could not open audio device. Audio playback is unsupported on this system.";
        case PAT_FILE_OPEN_ERROR:
            return "Could not open the audio file. The audio file is inaccessible or does not exist.";
        case PAT_DEMUX_ERROR:
            return "Could not demux the audio file. The audio file format is unsupported or corrupted, or the audio "
                   "file does not contain an audio stream.";
        case PAT_DECODE_ERROR:
            return "Could not decode audio stream. The audio stream codec is unsupported, the audio file does not "
                   "contain an audio stream, or the audio stream is corrupted.";
        case PAT_RESAMPLE_ERROR:
            return "Could not resample audio stream.";
        case PAT_MEMORY_ERROR:
            return "Memory allocation failed. The system does not have an adequate amount of memory. "
                   "Consider enabling the swap file";
        case PAT_UNKNOWN_ERROR:
            return "An unexpected error has occured. Please increase the log level to diagnose the error, and "
                   "report the error to the maintainers if it should be handled by PAT.";
    }
}
