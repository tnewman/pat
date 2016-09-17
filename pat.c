#include "pat.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <SDL2/SDL.h>

typedef struct PATAudioStream {
    AVStream* audio_stream;
    AVFormatContext* format_context;
    AVCodecContext* codec_context;
    SwrContext* resample_context;
} PATAudioStream;

SDL_AudioDeviceID output_device;
SDL_AudioSpec actual_output_spec;
int init_called = 0;

static PATError get_pat_audio_stream(const char* audio_file_path, PATAudioStream** pat_audio_stream);

static PATError play_pat_audio_stream(PATAudioStream* pat_audio_stream);

static PATError resample_pat_audio_stream(PATAudioStream* pat_audio_stream, AVFrame* frame,
    uint8_t** resampled_data, int* resampled_data_size);

static void free_pat_audio_stream(PATAudioStream** pat_audio_stream);

PATError pat_init(void) {
    if(SDL_Init(SDL_INIT_AUDIO) != 0) {
        return PAT_INIT_ERROR;
    }
    
    SDL_AudioSpec desired_output_spec;
    
    SDL_memset(&desired_output_spec, 0, sizeof(desired_output_spec));
    desired_output_spec.freq = 44100;
    desired_output_spec.format = AUDIO_S16SYS;
    desired_output_spec.channels = 2;
    desired_output_spec.silence = 0;
    desired_output_spec.samples = 1024;
    desired_output_spec.callback = NULL;
    desired_output_spec.userdata = NULL;
    
    output_device = SDL_OpenAudioDevice(NULL, 0, &desired_output_spec, 
        &actual_output_spec, 0);
    
    if(output_device == 0) {
        return PAT_OPEN_DEVICE_ERROR;
    }
    
    av_register_all();
    
    init_called = 1;
    
    return PAT_SUCCESS;
}

PATError pat_play(const char* audio_file_path) {
    PATAudioStream* pat_audio_stream = NULL;
    
    int result = get_pat_audio_stream(audio_file_path, &pat_audio_stream);
    
    if(result == PAT_SUCCESS) {
        result = play_pat_audio_stream(pat_audio_stream);
        
        if(result == PAT_SUCCESS) {
            while(SDL_GetQueuedAudioSize(output_device) > 0) {
                SDL_Delay(100);
            }
        }
    }
    
    SDL_ClearQueuedAudio(output_device);
    
    free_pat_audio_stream(&pat_audio_stream);
    
    return result;
}

PATError get_pat_audio_stream(const char* audio_file_path, PATAudioStream** pat_audio_stream) {
    free_pat_audio_stream(pat_audio_stream);    
    
    if(init_called == 0) {
        return PAT_NOT_INITIALIZED_ERROR;
    }
    
    *pat_audio_stream = malloc(sizeof(PATAudioStream));
    
    if(*pat_audio_stream == NULL) {
        return PAT_OPEN_FILE_ERROR;
    }
    
    (*pat_audio_stream)->audio_stream = NULL;
    (*pat_audio_stream)->format_context = NULL;
    (*pat_audio_stream)->codec_context = NULL;
    (*pat_audio_stream)->resample_context = NULL;
    
    // AVFormatContext
    AVFormatContext* format_context = NULL;
    
    if(avformat_open_input(&format_context, audio_file_path, NULL, NULL) != 0) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_OPEN_FILE_ERROR;
    }
    
    (*pat_audio_stream)->format_context = format_context;
    
    if(avformat_find_stream_info(format_context, NULL) < 0) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_OPEN_FILE_ERROR;
    }
    
    // AVStream
    AVStream* audio_stream = NULL;
    
    for(unsigned int i = 0; i < format_context->nb_streams; i++) {
        if(format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream = format_context->streams[i];
        }
    }
    
    if(audio_stream == NULL) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_NO_AUDIO_STREAM_ERROR;
    }
    
    (*pat_audio_stream)->audio_stream = audio_stream;
    
    // AVCodec
    AVCodecParameters* codec_parameters = audio_stream->codecpar;
    AVCodec* codec = avcodec_find_decoder(codec_parameters->codec_id);
    
    if(codec == NULL) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_CODEC_ERROR;
    }
    
    AVCodecContext* codec_context = avcodec_alloc_context3(codec);
    
    if(codec_context == NULL) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_CODEC_ERROR;
    }
    
    (*pat_audio_stream)->codec_context = codec_context;
    
    if(avcodec_parameters_to_context(codec_context, codec_parameters) < 0) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_CODEC_ERROR;
    }
    
    if(avcodec_open2(codec_context, codec_context->codec, NULL) != 0) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_CODEC_ERROR;
    }
    
    // Swr
    SwrContext* resample_context = swr_alloc_set_opts(NULL, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 
        actual_output_spec.freq, codec_context->channel_layout, codec_context->sample_fmt, 
        codec_context->sample_rate, 0, NULL);
    
    if(resample_context == NULL) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_RESAMPLE_ERROR;
    }
    
    (*pat_audio_stream)->resample_context = resample_context;
    
    if(swr_init(resample_context) != 0) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_RESAMPLE_ERROR;
    }
    
    return PAT_SUCCESS;
}

PATError play_pat_audio_stream(PATAudioStream* pat_audio_stream) {
    SDL_PauseAudioDevice(output_device, 0);
        
    AVPacket packet;
    av_init_packet(&packet);
    
    AVFrame* frame = av_frame_alloc();
    
    if(frame == NULL) {
        av_free(frame);
        return PAT_CODEC_ERROR;
    }
    
    while(av_read_frame(pat_audio_stream->format_context, &packet) == 0) {
        if(packet.stream_index == pat_audio_stream->audio_stream->index) {
            if(avcodec_send_packet(pat_audio_stream->codec_context, &packet) != 0) {
                av_free(frame);
                av_packet_unref(&packet);
                return PAT_CODEC_ERROR;
            }
        }
        
        if(avcodec_receive_frame(pat_audio_stream->codec_context, frame) != 0) {
            av_free(frame);
            av_packet_unref(&packet);
            return PAT_CODEC_ERROR;
        }
        
        uint8_t* resampled_data;
        int resampled_data_size;
        
        int result = resample_pat_audio_stream(pat_audio_stream, frame, &resampled_data, 
            &resampled_data_size);
        
        if(result != PAT_SUCCESS) {
            av_packet_unref(&packet);
            av_frame_unref(frame);
            av_free(frame);
            av_freep(&resampled_data);
            return result;
        }
        
        if(SDL_QueueAudio(output_device, resampled_data, (uint32_t) resampled_data_size) != 0) {
            av_packet_unref(&packet);
            av_frame_unref(frame);
            av_free(frame);
            av_freep(&resampled_data);
            return PAT_PLAYBACK_ERROR;
        }
        
        av_frame_unref(frame);
        av_freep(&resampled_data);
        av_packet_unref(&packet);
    }
    
    av_free(frame);
    
    return PAT_SUCCESS;
}

PATError resample_pat_audio_stream(PATAudioStream* pat_audio_stream, AVFrame* frame,
    uint8_t** resampled_data, int* resampled_data_size) {
    
    AVCodecContext* codec_context = pat_audio_stream->codec_context;
    
    if(av_samples_alloc(resampled_data, NULL, codec_context->channels, frame->nb_samples, 
        AV_SAMPLE_FMT_S16, 0) < 0) {
        return PAT_RESAMPLE_ERROR;
    }
    
    int resample_count = swr_convert(pat_audio_stream->resample_context, resampled_data, 
        frame->nb_samples, (const uint8_t **) frame->extended_data, frame->nb_samples);
    
    if(resample_count < 0) {
        return PAT_RESAMPLE_ERROR;
    }
    
    *resampled_data_size = av_samples_get_buffer_size(NULL, codec_context->channels, 
        resample_count, AV_SAMPLE_FMT_S16, 0);
    
    if(*resampled_data_size < 0) {
        return PAT_RESAMPLE_ERROR;
    }
    
    return PAT_SUCCESS;
}

void free_pat_audio_stream(PATAudioStream** pat_audio_stream) {
    if(*pat_audio_stream == NULL) {
        return;
    }
    
    AVCodecContext* codec_context = (*pat_audio_stream)->codec_context;
    
    if(codec_context != NULL) {
        avcodec_free_context(&codec_context);
    }
    
    AVFormatContext* format_context = (*pat_audio_stream)->format_context;
    
    if(format_context != NULL) {
        avformat_close_input(&format_context);
    }
    
    SwrContext* resample_context = (*pat_audio_stream)->resample_context;
    
    if(resample_context != NULL) {
        swr_free(&resample_context);
    }
    
    free(*pat_audio_stream);
    
    *pat_audio_stream = NULL;
}

void pat_quit(void) {
    SDL_CloseAudioDevice(output_device);
    
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    
    return;
}

