#include "pat_audio_device.h"
#include "pat_decode.h"
#include <stdlib.h>

int main(void) {
    PATAudioDevice* pat_audio_device = pat_open_audio_device();

    if(pat_audio_device == NULL) {
        return -1;
    }

    PATDecoder* pat_decoder = pat_open_audio_decoder(pat_audio_device, "/home/tnewman/Downloads/937354116178869607.mpga");

    if(pat_decoder == NULL) {
        pat_free_audio_device(pat_audio_device);
        return -2;
    }

    pat_decode_audio(pat_audio_device, pat_decoder);

    pat_free_audio_decoder(pat_decoder);
    pat_free_audio_device(pat_audio_device);

    return 0;
}