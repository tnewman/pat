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
sudo apt install build-essential cmake libavutil-dev libswresample-dev libavcodec-dev libavformat-dev libswscale-dev libsdl2-dev
```

## Build and Install
### Ubuntu
```bash
git clone https://github.com/tnewman/pat
cd pat

mkdir -p libpat/build
cd libpat/build
cmake ..
make

cd ../../pypat
python3 setup.py bdist_wheel
sudo pip3 install dist/pypat-0.1.0-cp36-cp36m-linux_x86_64.whl
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
