#!/bin/bash
cd ffmpeg
./configure --toolchain=msvc --enable-static
make clean
make -j8
find . -name "*.a" -type f -exec cp {} ../lib \;