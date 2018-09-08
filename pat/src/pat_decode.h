#include <stdint-gcc.h>

typedef struct PATDecoder PATDecoder;

PATDecoder* pat_open_audio_decoder(const char* audio_path);

int64_t pat_audio_decoder_get_stream_number(const PATDecoder* pat_decoder);

void pat_free_audio_decoder(PATDecoder* pat_decoder);
