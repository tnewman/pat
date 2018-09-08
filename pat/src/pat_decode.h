#include "pat_audio_device.h""

typedef struct PATDecoder PATDecoder;

PATDecoder* pat_open_audio_decoder(PATAudioDevice* pat_audio_device, const char* audio_path);

void pat_free_audio_decoder(PATDecoder* pat_decoder);
