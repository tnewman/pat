#include "pat/pat.h"
#include "pat/pat_error.h"
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Invalid Parameters\n");
		printf("Usage: test path/to/audio\n");
		return PAT_FILE_OPEN_ERROR;
	}

    PATError status;
    PAT* pat;

    status = pat_open(&pat);

    if(status != PAT_SUCCESS) {
        const char* error_string = pat_error_to_string(status);
        printf("%s", error_string);
        return status;
    }

    status = pat_play(pat, argv[1]);

    if(status != PAT_SUCCESS) {
        const char* error_string = pat_error_to_string(status);
        printf("%s\n", error_string);
    }

    pat_close(pat);

    return status;
}
