# libpat
libpat is a C library that makes playing audio simple. libpat uses FFmpeg to provide 
robust support for virtually any audio format and SDL2 to play back audio.

## Prerequisites
Install [Prerequisites](https://github.com/tnewman/pat#Prerequisites) before building.

## Build
```bash
cd build
cmake ..
cmake --build .
```

After the build, build/bin will contain the libpat shared library that can be used in your 
application.

## Example
```c
#include "pat/pat.h"
#include "pat/pat_error.h"
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Invalid Parameters\n");
		printf("Usage: patplay path/to/audio\n");
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
```
