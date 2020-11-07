#include "pat/pat.h"
#include "pat/pat_error.h"
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Invalid Parameters\n");
		printf("Usage: patplay path/to/audio\n");
		return PAT_FILE_OPEN_ERROR;
	}

    PATError status = pat_init();

    if(status != PAT_SUCCESS) {
        const char* error_string = pat_error_to_string(status);
        printf("%s", error_string);
        return status;
    }

    status = pat_play(argv[1]);

    if(status != PAT_SUCCESS) {
        const char* error_string = pat_error_to_string(status);
        printf("%s\n", error_string);
    }

    return status;
}
