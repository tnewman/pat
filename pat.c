#include "pat.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <SDL2/SDL.h>

typedef struct PATAudioStream {
    AVStream* audio_stream;
    AVFormatContext* format_context;
    AVCodecContext* codec_context;
} PATAudioStream;

SDL_AudioDeviceID output_device;
SDL_AudioSpec actual_output_spec;
int init_called = 0;

int get_pat_audio_stream(const char* audio_file_path, PATAudioStream** pat_audio_stream);

void free_pat_audio_stream(PATAudioStream** pat_audio_stream);

int pat_init() {
    if(SDL_Init(SDL_INIT_AUDIO) != 0) {
        return PAT_INIT_ERROR;
    }
    
    SDL_AudioSpec desired_output_spec;
    
    SDL_memset(&desired_output_spec, 0, sizeof(desired_output_spec));
    desired_output_spec.freq = 48000;
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

int get_pat_audio_stream(const char* audio_file_path, PATAudioStream** pat_audio_stream) {
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
    
    // AVFormatContext
    AVFormatContext* format_context = NULL;
    
    if(avformat_open_input(&format_context, audio_file_path, NULL, NULL) != 0) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_OPEN_FILE_ERROR;
    }
    
    (*pat_audio_stream)->format_context = format_context;
    
    if(avformat_find_stream_info(format_context, NULL) != 0) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_OPEN_FILE_ERROR;
    }
    
    // AVStream
    AVStream* audio_stream = NULL;
    
    for(int i = 0; i < format_context->nb_streams; i++) {
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
    
    if(avcodec_parameters_to_context(codec_context, codec_parameters) < 0) {
        free_pat_audio_stream(pat_audio_stream);
        return PAT_CODEC_ERROR;
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
    
    free(*pat_audio_stream);
    
    *pat_audio_stream = NULL;
}

int pat_quit() {
    SDL_CloseAudioDevice(output_device);
    
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    
    return 0;
}

int main() {

}
