#!/bin/bash
cd sdl
./configure --enable-static=yes --enable-shared=no
make clean
make -j8
cp build/.libs/libSDL2.a ../lib
