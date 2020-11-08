# PAT (PAT Audio Technician)
![PAT CI/CD](https://github.com/tnewman/pat/workflows/PAT%20CI/CD/badge.svg)

PAT is a multi-programming language audio library that makes playing audio simple. Behind the 
scenes, PAT uses FFmpeg to provide robust support for virtually any audio format and SDL2 to 
play back audio.

## Supported Platforms
- Linux
- Mac OS
- Windows

## Programming Languages
- [C/C++ (libpat)](libpat/README.md)
- [NodeJS (nodepat)](nodepat/README.md)
- [Python (pypat)](pypat/README.md)
- [Rust (rustpat)](rustpat/README.md)

## Prerequisites
- [FFmpeg](https://www.ffmpeg.org/download.html)
- [SDL2](https://www.libsdl.org/download-2.0.php)
- Standard C Build Tools (gcc, Xcode, Visual Studio, etc.)

### Ubuntu
```bash
sudo apt install build-essential cmake libavutil-dev libswresample-dev libavdevice-dev libavcodec-dev libavformat-dev libswscale-dev libsdl2-dev ninja-build
```

### Mac OS
```bash
brew install ffmpeg sdl2
```
