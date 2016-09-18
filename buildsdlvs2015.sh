#!/bin/bash
cd sdl
rm -rf CMakeCache.txt CMakeFiles build
mkdir build
cd build
cmake .. -G"Visual Studio 14 2015 Win64" -DSDL_SHARED_ENABLED_BY_DEFAULT=OFF
MSBuild.exe SDL2.sln /property:Configuration=Release
cp Release/SDL2.lib ../../lib
