#include "pat_decode.h"
#include <SDL.h>
#include <stdlib.h>

static AVFormatContext* pat_open_format_context(const char* audio_path);

static AVCodecContext* pat_open_codec_context(AVStream* stream);

static SwrContext* pat_open_swr_context(PATAudioDevice* pat_audio_device, AVCodecContext* decoder_context);

static enum AVSampleFormat pat_get_ffmpeg_sample_format(uint16_t format);

static void pat_resample_frame(const PATAudioDevice* pat_audio_device, const PATDecoder* pat_decoder,
        enum AVSampleFormat format, AVFrame* av_frame);

static void pat_flush(const PATAudioDevice *pat_audio_device, const PATDecoder *pat_decoder,
        enum AVSampleFormat format, AVPacket *av_packet, AVFrame *av_frame);

void pat_init_audio_decoder() {
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
    av_log_set_level(AV_LOG_QUIET);
}

PATDecoder* pat_open_audio_decoder(PATAudioDevice* pat_audio_device, const char* audio_path) {
    AVFormatContext* format_context = pat_open_format_context(audio_path);

    if(format_context == NULL) {
        return NULL;
    }

    int64_t stream_index = av_find_best_stream(format_context, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

    if(stream_index < 0) {
        avformat_close_input(&format_context);
        return NULL;
    }

    AVStream* stream = format_context->streams[stream_index];

    AVCodecContext* decoder_context = pat_open_codec_context(stream);

    if(decoder_context == NULL) {
        avformat_close_input(&format_context);
        return NULL;
    }

    SwrContext* swr_context = pat_open_swr_context(pat_audio_device, decoder_context);

    if(swr_context == NULL) {
        avformat_close_input(&format_context);
        avcodec_free_context(&decoder_context);
        return NULL;
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
        return NULL;
    }

    if(avformat_find_stream_info(format_context, NULL) < 0) {
        avformat_close_input(&format_context);
        return NULL;
    }

    return format_context;
}

static AVCodecContext* pat_open_codec_context(AVStream* stream) {
    AVCodec* decoder = avcodec_find_decoder(stream->codecpar->codec_id);

    if(decoder == NULL) {
        return NULL;
    }

    AVCodecContext* decoder_context = avcodec_alloc_context3(decoder);

    if(decoder_context == NULL) {
        return NULL;
    }

    if(avcodec_parameters_to_context(decoder_context, stream->codecpar) < 0) {
        avcodec_free_context(&decoder_context);
    }

    if(avcodec_open2(decoder_context, decoder_context->codec, NULL) < 0) {
        avcodec_free_context(&decoder_context);
    }

    return decoder_context;
}

static SwrContext* pat_open_swr_context(PATAudioDevice* pat_audio_device, AVCodecContext* decoder_context) {
    int64_t out_channel_layout = av_get_default_channel_layout(pat_audio_device->channels);

    enum AVSampleFormat out_format = pat_get_ffmpeg_sample_format(pat_audio_device->format);

    SwrContext* swr_context = swr_alloc_set_opts(NULL, out_channel_layout, out_format, pat_audio_device->frequency,
            decoder_context->channel_layout, decoder_context->sample_fmt, decoder_context->sample_rate, 0, NULL);

    if(swr_context == NULL) {
        return NULL;
    }

    if(swr_init(swr_context) != 0) {
        swr_free(&swr_context);
        return NULL;
    }

    return swr_context;
}

static enum AVSampleFormat pat_get_ffmpeg_sample_format(const uint16_t format) {
    switch(format) {
        case AUDIO_U8:
            return AV_SAMPLE_FMT_U8;
        case AUDIO_S16SYS:
            return AV_SAMPLE_FMT_S16;
        case AUDIO_S32SYS:
        default:
            return AV_SAMPLE_FMT_S32;
    }
}

void pat_decode_audio(PATAudioDevice* pat_audio_device, PATDecoder* pat_decoder) {
    enum AVSampleFormat format = pat_get_ffmpeg_sample_format(pat_audio_device->format);

    AVPacket av_packet;
    av_init_packet(&av_packet);

    AVFrame* av_frame = av_frame_alloc();

    if(av_frame == NULL) {
        return;
    }

    while(av_read_frame(pat_decoder->format_context, &av_packet) == 0) {
        if(av_packet.stream_index == pat_decoder->stream_index) {
            if (avcodec_send_packet(pat_decoder->decoder_context, &av_packet) != 0) {
                av_packet_unref(&av_packet);
                av_frame_unref(av_frame);
                av_frame_free(&av_frame);
                return;
            }

            if(avcodec_receive_frame(pat_decoder->decoder_context, av_frame) != 0) {
                av_packet_unref(&av_packet);
                av_frame_unref(av_frame);
                av_frame_free(&av_frame);
                return;
            }

            pat_resample_frame(pat_audio_device, pat_decoder, format, av_frame);
        }

        av_packet_unref(&av_packet);
        av_frame_unref(av_frame);
    }

    pat_flush(pat_audio_device, pat_decoder, format, &av_packet, av_frame);
}

static void pat_flush(const PATAudioDevice *pat_audio_device, const PATDecoder *pat_decoder,
        enum AVSampleFormat format, AVPacket *av_packet, AVFrame *av_frame) {
    av_packet->data = NULL;
    av_packet->size = 0;

    avcodec_send_packet(pat_decoder->decoder_context, av_packet);

    while(avcodec_receive_frame(pat_decoder->decoder_context, av_frame) == 0) {
        pat_resample_frame(pat_audio_device, pat_decoder, format, av_frame);
        av_frame_unref(av_frame);
    }

    av_frame->extended_data = NULL;
    av_frame->nb_samples = 0;

    while(swr_get_delay(pat_decoder->swr_context, 1000) > 0) {
        pat_resample_frame(pat_audio_device, pat_decoder, format, av_frame);
    }

    av_frame_free(&av_frame);
}

static void pat_resample_frame(const PATAudioDevice* pat_audio_device, const PATDecoder* pat_decoder,
        const enum AVSampleFormat format, AVFrame* av_frame) {
    uint8_t* resampled_data = NULL;

    // https://www.ffmpeg.org/doxygen/3.1/group__lswr.html documents the output samples calculation
    int64_t output_samples = av_rescale_rnd(swr_get_delay(pat_decoder->swr_context,
            pat_decoder->decoder_context->sample_rate) + av_frame->nb_samples, pat_audio_device->frequency,
                    pat_decoder->decoder_context->sample_rate, AV_ROUND_UP);

    if(av_samples_alloc(&resampled_data, NULL, pat_audio_device->channels, (int) output_samples,
                        format, 0) < 0) {
        return;
    }

    output_samples = swr_convert(pat_decoder->swr_context, &resampled_data, (int) output_samples,
                                 (const uint8_t **) av_frame->extended_data, av_frame->nb_samples);

    if(output_samples < 0) {
        av_freep(&resampled_data);
        return;
    }

    if(output_samples == 0) {
        av_freep(&resampled_data);
        return;
    }

    int64_t buffer_size = output_samples * pat_audio_device->channels * av_get_bytes_per_sample(format);
    int64_t written;

    do {
        written = pat_write_ring_buffer(pat_audio_device->pat_ring_buffer, resampled_data,
                                        (size_t) buffer_size, 100);
    } while(written == 0);

    av_freep(&resampled_data);
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
