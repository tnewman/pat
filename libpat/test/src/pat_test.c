#include "pat/pat.h"
#include "pat/pat_error.h"
#include <stdio.h>

int main(void) {
    PATError status;
    PAT* pat;

    status = pat_open(&pat);

    if(status != PAT_SUCCESS) {
        const char* error_string = pat_error_to_string(status);
        printf("%s", error_string);
        return status;
    }

    status = pat_play(pat, "/home/tnewman/Downloads/test.mp4");

    if(status != PAT_SUCCESS) {
        const char* error_string = pat_error_to_string(status);
        printf("%s", error_string);
    }

    pat_close(pat);

    return status;
}
