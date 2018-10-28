#include "pat/pat.h"
#include <stdlib.h>

int main(void) {
    PAT* pat = pat_open();

    if(pat == NULL) {

    }

    pat_play(pat, "/home/tnewman/Downloads/tilltheend.mpga");
    
    pat_close(pat);
    return 0;
}
