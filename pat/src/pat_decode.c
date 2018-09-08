#include "pat_decode.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <stdio.h>

typedef struct PATDecoder {
    AVFormatContext* format_context;
    AVCodecContext* decoder_context;
    int64_t stream_index;
} PATDecoder;

static AVFormatContext* pat_open_format_context(const char* audio_path);

static AVCodecContext* pat_open_codec_context(AVCodec* decoder);

PATDecoder* pat_open_audio_decoder(const char* audio_path) {
    av_register_all();
    avformat_network_init();

    AVFormatContext* format_context = pat_open_format_context(audio_path);

    if(format_context == NULL) {
        return NULL;
    }

    AVCodec* decoder = NULL;

    int64_t stream_index = av_find_best_stream(format_context, AVMEDIA_TYPE_AUDIO, -1, -1, &decoder, 0);

    if(stream_index < 0) {
        avformat_close_input(&format_context);
        return NULL;
    }

    AVCodecContext* decoder_context = pat_open_codec_context(decoder);

    if(decoder_context == NULL) {
        avformat_close_input(&format_context);
    }

    PATDecoder* pat_decoder = malloc(sizeof(PATDecoder));
    pat_decoder->format_context = format_context;
    pat_decoder->decoder_context = decoder_context;
    pat_decoder->stream_index = stream_index;

    return pat_decoder;
}

static AVFormatContext* pat_open_format_context(const char* audio_path) {
    AVFormatContext* format_context = NULL;

    if(avformat_open_input(&format_context, audio_path, NULL, NULL) != 0) {
        printf("Could not open %s\n", audio_path);
        return NULL;
    }

    printf("Opened %s\n", audio_path);

    if(avformat_find_stream_info(format_context, NULL) < 0) {
        printf("Could not find stream info in %s\n", audio_path);
        avformat_close_input(&format_context);
        return NULL;
    }

    return format_context;
}

static AVCodecContext* pat_open_codec_context(AVCodec* decoder) {
    AVCodecContext* decoder_context = avcodec_alloc_context3(decoder);

    if(decoder_context == NULL) {
        return NULL;
    }

    if(avcodec_open2(decoder_context, decoder_context->codec, NULL) < 0) {
        avcodec_free_context(&decoder_context);
    }

    return decoder_context;
}

void pat_free_audio_decoder(PATDecoder* pat_decoder) {
    if(pat_decoder == NULL) {
        return;
    }

    if(pat_decoder->format_context != NULL) {
        avformat_close_input(&pat_decoder->format_context);
    }

    if(pat_decoder->decoder_context != NULL) {
        avcodec_free_context(&pat_decoder->decoder_context);
    }

    free(pat_decoder);
}
