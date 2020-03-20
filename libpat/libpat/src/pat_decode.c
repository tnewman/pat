#include "pat/pat_error.h"
#include "pat_decode.h"
#include <SDL.h>
#include <stdlib.h>

static PATError pat_open_audio_decoder(PATDecoder** pat_decoder_out, PATAudioDevice* pat_audio_device,
        const char* audio_path);

static PATError pat_run_audio_decoder(PATDecoder* pat_decoder, PATAudioDevice* pat_audio_device);

static void pat_free_audio_decoder(PATDecoder* pat_decoder);

static PATError pat_open_format_context(AVFormatContext** format_context_out, const char* audio_path);

static PATError pat_open_codec_context(AVCodecContext** decoder_context_out, AVStream* stream);

static PATError pat_open_swr_context(SwrContext** swr_context_out, PATAudioDevice* pat_audio_device,
        AVCodecContext* decoder_context);

static enum AVSampleFormat pat_get_ffmpeg_sample_format(const uint16_t format);

static PATError pat_resample_frame(PATAudioDevice* pat_audio_device, PATDecoder* pat_decoder,
        enum AVSampleFormat format, AVFrame* av_frame);

static void pat_flush(PATAudioDevice* pat_audio_device, PATDecoder *pat_decoder,
        enum AVSampleFormat format, AVPacket *av_packet, AVFrame *av_frame);

#define PACKET_BUFFER_SIZE 1024

void pat_init_audio_decoder() {
    avformat_network_init();
    av_log_set_level(AV_LOG_QUIET);
}

PATError pat_decode_audio(PATAudioDevice* pat_audio_device, const char* audio_path) {
    SDL_AtomicAdd(&pat_audio_device->audio_wait_count, 1);
    SDL_LockMutex(pat_audio_device->audio_lock);
    SDL_AtomicAdd(&pat_audio_device->audio_wait_count, -1);

    PATError status;

    if(audio_path == NULL || SDL_AtomicGet(&pat_audio_device->audio_wait_count)) {
        status = PAT_SUCCESS;
    } else {
        PATDecoder *pat_decoder;

        status = pat_open_audio_decoder(&pat_decoder, pat_audio_device, audio_path);

        if(status == PAT_SUCCESS) {
            status = pat_run_audio_decoder(pat_decoder, pat_audio_device);
            pat_free_audio_decoder(pat_decoder);
        }
    }

    SDL_UnlockMutex(pat_audio_device->audio_lock);

    return status;
}

PATError pat_skip_audio(PATAudioDevice* pat_audio_device) {
    pat_decode_audio(pat_audio_device, NULL);
    return PAT_SUCCESS;
}

static PATError pat_open_audio_decoder(PATDecoder** pat_decoder_out, PATAudioDevice* pat_audio_device,
                                       const char* audio_path) {
    *pat_decoder_out = NULL;
    PATError status;

    AVFormatContext* format_context;

    if((status = pat_open_format_context(&format_context, audio_path)) != PAT_SUCCESS) {
        *pat_decoder_out = NULL;
        return status;
    }

    int64_t stream_index = av_find_best_stream(format_context, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

    if(stream_index < 0) {
        avformat_close_input(&format_context);
        return PAT_DEMUX_ERROR;
    }

    AVStream* stream = format_context->streams[stream_index];
    AVCodecContext* decoder_context = NULL;

    if((status = pat_open_codec_context(&decoder_context, stream)) != PAT_SUCCESS) {
        avformat_close_input(&format_context);
        return status;
    }

    SwrContext* swr_context = NULL;

    if((status = pat_open_swr_context(&swr_context, pat_audio_device, decoder_context)) != PAT_SUCCESS) {
        avformat_close_input(&format_context);
        avcodec_free_context(&decoder_context);
        return PAT_RESAMPLE_ERROR;
    }

    PATDecoder* pat_decoder = malloc(sizeof(PATDecoder));

    if(pat_decoder == NULL) {
        return PAT_MEMORY_ERROR;
    }

    pat_decoder->format_context = format_context;
    pat_decoder->decoder_context = decoder_context;
    pat_decoder->swr_context = swr_context;
    pat_decoder->stream_index = stream_index;

    *pat_decoder_out = pat_decoder;

    return status;
}

static PATError pat_open_format_context(AVFormatContext** format_context_out, const char* audio_path) {
    int status = 0;
    AVFormatContext* format_context = NULL;

    if((status = avformat_open_input(&format_context, audio_path, NULL, NULL)) != 0) {
        *format_context_out = NULL;

        if(status == AVERROR(ENOENT) || status == AVERROR(EACCES) || status == AVERROR(EIO)) {
            return PAT_FILE_OPEN_ERROR;
        } else {
            return PAT_DEMUX_ERROR;
        }
    }

    if(avformat_find_stream_info(format_context, NULL) < 0) {
        *format_context_out = NULL;
        avformat_close_input(&format_context);
        return PAT_DEMUX_ERROR;
    }

    *format_context_out = format_context;

    return PAT_SUCCESS;
}

static PATError pat_open_codec_context(AVCodecContext** decoder_context_out, AVStream* stream) {
    AVCodec* decoder = avcodec_find_decoder(stream->codecpar->codec_id);

    if(decoder == NULL) {
        *decoder_context_out = NULL;
        return PAT_DECODE_ERROR;
    }

    AVCodecContext* decoder_context = avcodec_alloc_context3(decoder);

    if(decoder_context == NULL) {
        *decoder_context_out = NULL;
        return PAT_MEMORY_ERROR;
    }

    if(avcodec_parameters_to_context(decoder_context, stream->codecpar) < 0) {
        *decoder_context_out = NULL;
        avcodec_free_context(&decoder_context);
        return PAT_DECODE_ERROR;
    }

    if(avcodec_open2(decoder_context, decoder_context->codec, NULL) < 0) {
        *decoder_context_out = NULL;
        avcodec_free_context(&decoder_context);
        return PAT_DECODE_ERROR;
    }

    *decoder_context_out = decoder_context;
    return PAT_SUCCESS;
}

static PATError pat_open_swr_context(SwrContext** swr_context_out, PATAudioDevice* pat_audio_device,
                                     AVCodecContext* decoder_context) {
    *swr_context_out = NULL;
    int64_t out_channel_layout = av_get_default_channel_layout(pat_audio_device->channels);

    enum AVSampleFormat out_format = pat_get_ffmpeg_sample_format(pat_audio_device->format);

    SwrContext* swr_context = swr_alloc_set_opts(NULL, out_channel_layout, out_format, pat_audio_device->frequency,
                                                 decoder_context->channel_layout, decoder_context->sample_fmt,
                                                 decoder_context->sample_rate, 0, NULL);

    if(swr_context == NULL) {
        return PAT_RESAMPLE_ERROR;
    }

    if(swr_init(swr_context) != 0) {
        swr_free(&swr_context);
        return PAT_RESAMPLE_ERROR;
    }

    *swr_context_out = swr_context;

    return PAT_SUCCESS;
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

static PATError pat_run_audio_decoder(PATDecoder* pat_decoder, PATAudioDevice* pat_audio_device) {
    PATError status = PAT_SUCCESS;
    enum AVSampleFormat format = pat_get_ffmpeg_sample_format(pat_audio_device->format);

    AVPacket av_packet;
    uint8_t packet_buffer[PACKET_BUFFER_SIZE];
    av_packet.data = packet_buffer;
    av_packet.size = PACKET_BUFFER_SIZE;

    AVFrame* av_frame = av_frame_alloc();

    if(av_frame == NULL) {
        return PAT_MEMORY_ERROR;
    }

    while(av_read_frame(pat_decoder->format_context, &av_packet) == 0 &&
    SDL_AtomicGet(&pat_audio_device->audio_wait_count) == 0) {

        if(av_packet.stream_index == pat_decoder->stream_index) {
            if (avcodec_send_packet(pat_decoder->decoder_context, &av_packet) != 0) {
                av_packet_unref(&av_packet);
                av_frame_unref(av_frame);
                av_frame_free(&av_frame);
                return PAT_DECODE_ERROR;
            }

            int receive_frame_error;

            if((receive_frame_error = avcodec_receive_frame(pat_decoder->decoder_context, av_frame)) != 0) {
                av_packet_unref(&av_packet);
                av_frame_unref(av_frame);

                if(receive_frame_error == AVERROR(EAGAIN)) {
                    continue;
                }

                av_frame_free(&av_frame);
                return PAT_DECODE_ERROR;
            }

            if((status = pat_resample_frame(pat_audio_device, pat_decoder, format, av_frame)) != PAT_SUCCESS) {
                av_packet_unref(&av_packet);
                av_frame_unref(av_frame);
                av_frame_free(&av_frame);
                return status;
            }
        }

        av_packet_unref(&av_packet);
        av_frame_unref(av_frame);
    }

    if(SDL_AtomicGet(&pat_audio_device->audio_wait_count) != 0) {
        pat_clear_ring_buffer(pat_audio_device->pat_ring_buffer);
    } else {
        pat_flush(pat_audio_device, pat_decoder, format, &av_packet, av_frame);
    }

    av_packet_unref(&av_packet);
    av_frame_unref(av_frame);
    av_frame_free(&av_frame);

    return status;
}

static void pat_flush(PATAudioDevice *pat_audio_device, PATDecoder *pat_decoder, enum AVSampleFormat format,
        AVPacket *av_packet, AVFrame *av_frame) {
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
}

static PATError pat_resample_frame(PATAudioDevice* pat_audio_device, PATDecoder* pat_decoder,
        const enum AVSampleFormat format, AVFrame* av_frame) {
    uint8_t* resampled_data = NULL;

    // https://www.ffmpeg.org/doxygen/3.1/group__lswr.html documents the output samples calculation
    int64_t output_samples = av_rescale_rnd(swr_get_delay(pat_decoder->swr_context,
            pat_decoder->decoder_context->sample_rate) + av_frame->nb_samples, pat_audio_device->frequency,
                    pat_decoder->decoder_context->sample_rate, AV_ROUND_UP);

    if(av_samples_alloc(&resampled_data, NULL, pat_audio_device->channels, (int) output_samples,
                        format, 0) < 0) {
        return PAT_MEMORY_ERROR;
    }

    output_samples = swr_convert(pat_decoder->swr_context, &resampled_data, (int) output_samples,
                                 (const uint8_t **) av_frame->extended_data, av_frame->nb_samples);

    if(output_samples < 0) {
        av_freep(&resampled_data);
        return PAT_RESAMPLE_ERROR;
    }

    if(output_samples == 0) {
        av_freep(&resampled_data);
        return PAT_SUCCESS;
    }

    int64_t buffer_size = output_samples * pat_audio_device->channels * av_get_bytes_per_sample(format);
    int64_t written;

    do {
        written = pat_write_ring_buffer(pat_audio_device->pat_ring_buffer, resampled_data,
                                        (size_t) buffer_size, 500);
    } while(written == 0 && SDL_AtomicGet(&pat_audio_device->audio_wait_count) == 0);

    av_freep(&resampled_data);

    return PAT_SUCCESS;
}

static void pat_free_audio_decoder(PATDecoder* pat_decoder) {
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
