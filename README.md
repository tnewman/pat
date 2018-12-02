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
sudo apt install build-essential cmake ninja-build libavutil-dev libswresample-dev libavdevice-dev libavcodec-dev libavformat-dev libswscale-dev libsdl2-dev
```

## Build and Install
### Ubuntu
#### pypat
```bash
git clone https://github.com/tnewman/pat
cd pat/pypat
sudo python3 setup.py install
```

## Examples
### pypat
```python
>>> import pypat

# Play a file
>>> pypat.play('amerika.mpga')

# Skip the current file
>>> pypat.skip()

# Pause audio playback
>>> pypat.pause()

# Resume audio playback
>>> pypat.resume()
```
