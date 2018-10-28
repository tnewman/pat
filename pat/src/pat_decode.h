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

PATDecoder* pat_open_audio_decoder(PATAudioDevice* pat_audio_device, const char* audio_path);

void pat_decode_audio(PATAudioDevice* pat_audio_device, PATDecoder* pat_decoder);

void pat_free_audio_decoder(PATDecoder* pat_decoder);

#endif
