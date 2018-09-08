#include "pat_decode.h"

int main(void) {
    PATDecoder* pat_decoder = pat_open_audio_decoder("/home/tnewman/Downloads/bulletproof.aac");
    pat_free_audio_decoder(pat_decoder);
    return 0;
}