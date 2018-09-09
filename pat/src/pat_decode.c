#include "pat_decode.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "SDL.h"
#include <stdio.h>

typedef struct PATDecoder {
    AVFormatContext* format_context;
    AVCodecContext* decoder_context;
    SwrContext* swr_context;
    int64_t stream_index;
} PATDecoder;

static AVFormatContext* pat_open_format_context(const char* audio_path);

static AVCodecContext* pat_open_codec_context(AVCodec* decoder);

static SwrContext* pat_open_swr_context(PATAudioDevice* pat_audio_device, AVCodecContext* decoder_context);

PATDecoder* pat_open_audio_decoder(PATAudioDevice* pat_audio_device, const char* audio_path) {
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

    SwrContext* swr_context = pat_open_swr_context(pat_audio_device, decoder_context);

    if(swr_context == NULL) {
        avformat_close_input(&format_context);
        avcodec_free_context(&decoder_context);
    }

    PATDecoder* pat_decoder = malloc(sizeof(PATDecoder));
    pat_decoder->format_context = format_context;
    pat_decoder->decoder_context = decoder_context;
    pat_decoder->swr_context = swr_context;
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

static SwrContext* pat_open_swr_context(PATAudioDevice* pat_audio_device, AVCodecContext* decoder_context) {
    int64_t out_channel_layout;

    switch(pat_audio_device->channels) {
        case 1:
            out_channel_layout = AV_CH_LAYOUT_MONO;
            break;
        case 2:
            out_channel_layout = AV_CH_LAYOUT_STEREO;
            break;
        case 4:
            out_channel_layout = AV_CH_LAYOUT_QUAD;
            break;
        case 6:
            out_channel_layout = AV_CH_LAYOUT_5POINT1;
            break;
        default:
            return NULL;
    }

    enum AVSampleFormat out_format;

    switch(pat_audio_device->format) {
        case AUDIO_U8:
            out_format = AV_SAMPLE_FMT_U8;
            break;
        case AUDIO_S16SYS:
            out_format = AV_SAMPLE_FMT_S16;
            break;
        case AUDIO_S32SYS:
            out_format = AV_SAMPLE_FMT_S32;
            break;
        default:
            return NULL;
    }

    SwrContext* swr_context = swr_alloc_set_opts(NULL, out_channel_layout, out_format, pat_audio_device->frequency,
            decoder_context->channel_layout, decoder_context->sample_fmt, decoder_context->sample_rate, 0, NULL);

    if(swr_context == NULL) {
        return NULL;
    }

    return swr_context;
}

void pat_decode_audio(PATAudioDevice* pat_audio_device, PATDecoder* pat_decoder) {

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

    if(pat_decoder->swr_context != NULL) {
        swr_free(&pat_decoder->swr_context);
    }

    free(pat_decoder);
}
