# PAT (PAT Audio Technician)
PAT is an audio library that makes playing audio simple. Behind the scenes, PAT uses FFmpeg 
to provide robust support for virtually any audio format and SDL to play back audio.

## Programming Languages
- C/C++ (libpat)
- Python (pypat)

## Supported Platforms
- Linux
- Other Platforms to Come

## Prerequisites
### Ubuntu
```bash
sudo apt install ffmpeg libsdl2-2.0
```

## Examples
### pypat
```python
>>> import pypat

# Play a file
>>> pypat.play('amerika.mpga')

# Skip the current song
>>> pypat.skip()
```

## Development Prerequisites
### Ubuntu
```bash
sudo apt install build-essential cmake libavutil-dev libswresample-dev libavcodec-dev libavformat-dev libswscale-dev libsdl2-dev
```


