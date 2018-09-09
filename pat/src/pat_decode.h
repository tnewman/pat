#ifndef PAT_DECODE_H
#define PAT_DECODE_H

#include "pat_audio_device.h"
#include "pat_ring_buffer.h"

typedef struct PATDecoder PATDecoder;

PATDecoder* pat_open_audio_decoder(PATAudioDevice* pat_audio_device, const char* audio_path);

void pat_decode_audio(PATAudioDevice* pat_audio_device, PATDecoder* pat_decoder);

void pat_free_audio_decoder(PATDecoder* pat_decoder);

#endif
