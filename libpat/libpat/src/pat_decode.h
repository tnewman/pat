#ifndef PAT_DECODE_H
#define PAT_DECODE_H

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>

#include "pat_audio_device.h"

typedef struct PATDecoder {
    AVFormatContext* format_context;
    AVCodecContext* decoder_context;
    SwrContext* swr_context;
    int64_t stream_index;
} PATDecoder;

void pat_init_audio_decoder();

PATError pat_decode_audio(PATAudioDevice* pat_audio_device, const char* audio_path);

PATError pat_skip_audio(PATAudioDevice* pat_audio_device);

#endif
