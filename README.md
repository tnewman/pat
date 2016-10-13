# PAT Audio Technician (PAT)
PAT is a Python library used to play audio files. If you have audio files and want to play them using 
Python, PAT the Audio Technician is at your service.

## How does it work?
PAT uses ffmpeg for audio decoding and resampling. SDL is used to play audio using the sound card. 
Everything is written in C, but the Python C Extension interface allows PAT to be used through the 
CPython interpreter.

## How do I install it?

### Warning
It is very difficult to get PAT working on Windows. Detailed instructions will 
be provided at a later date.

### Installation
- [Windows Installation Instructions](https://github.com/tnewman/pat/blob/master/INSTALL_WINDOWS.md)
- [Linux Installation Instructions](https://github.com/tnewman/pat/blob/master/INSTALL_LINUX.md)

## How do I use it?
PAT is currently under very active development, and the API WILL BREAK on occasion; however, the 
core functionality to play audio files is there.
```python
>>> import pat

# Queue an Audio File for Playback
>>> pat.queue('deerdance.m4a')

# Find Out How Many Bytes are Queued in the Audio Buffer
>>> pat.get_queue_len()
41848832

# Stop Playback
>>> pat.stop()
```

## Can PAT be used with programming languages other than Python?
All of the Python-specific code is just a wrapper, so porting PAT would be relatively easy. If 
you are using C/C++, you can use the functions declared in pat.h and link with ffmpeg. Any other 
programming languages can use the Python wrapper as a guide for porting efforts.

## Licensing Considerations
You must link against an LGPL version of ffmpeg if you wish to bound by the terms of the LGPL.

