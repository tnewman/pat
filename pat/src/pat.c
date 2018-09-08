#include "pat_decode.h"
#include "pat_audio_device.h"

int main(void) {
    PATAudioDevice* pat_audio_device = pat_open_audio_device();
    pat_free_audio_device(pat_audio_device);
    return 0;
}