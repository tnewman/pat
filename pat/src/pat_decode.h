typedef struct PATDecoder PATDecoder;

PATDecoder* pat_open_audio_decoder(const char* audio_path);

void pat_free_audio_decoder(PATDecoder* pat_decoder);
