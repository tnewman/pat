#include "pat/pat.h"

int main(void) {
    PAT* pat = pat_open();
    pat_play(pat, "/home/tnewman/Downloads/tilltheend.mpga");
    pat_close(pat);
    return 0;
}
