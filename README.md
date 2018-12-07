# PAT (PAT Audio Technician)
PAT is a multi-programming language audio library that makes playing audio simple. Behind the 
scenes, PAT uses FFmpeg to provide robust support for virtually any audio format and SDL2 to 
play back audio.

## Supported Platforms
- Linux
- Mac OSX
- Windows

## Programming Languages
- [C/C++ (libpat)](libpat/README.md)
- [Python (pypat)](pypat/README.md)

## Prerequisites
- [CMake](https://cmake.org/download/)
- [FFmpeg](https://www.ffmpeg.org/download.html)
- [Ninja](https://github.com/ninja-build/ninja/releases)
- [SDL2](https://www.libsdl.org/download-2.0.php)
- Standard C Build Tools (gcc, Xcode, Visual Studio, etc.)

## Windows-Specific Instructions
- All builds and installations need to be run from the 64 bit Visual Studio command prompt.
- DLLs for FFmpeg and SDL2 need to be on the PATH
- The environment variables `FFMPEGDIR` and `SDLDIR` need to be set to the root directories 
  for FFmpeg and SDL2, respectively.
