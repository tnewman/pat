#include "pat/pat.h"
#include "libavcodec/avcodec.h"
#include <stdio.h>

void test() {
    printf("%s", avcodec_license());
}
