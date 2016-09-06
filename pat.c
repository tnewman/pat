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

int get_audio_format_context(const char* audio_file_path);

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
    
    return PAT_SUCCESS;
}

int get_audio_stream(const char* audio_file_path) {
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
}

int pat_quit() {
    SDL_CloseAudioDevice(output_device);
    
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    
    return 0;
}

int main() {
    int retval;
    
    retval = pat_init();
    
    printf("%d", retval);
    
    pat_quit();
}
