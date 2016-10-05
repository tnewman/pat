# PAT Audio Technician (PAT)
PAT is a Python library used to play audio files. If you have audio files and want to play them using 
Python, PAT the Audio Technician is at your service.

## How does it work?
PAT uses ffmpeg for audio decoding and resampling. SDL is used to play audio using the sound card. 
Everything is written in C, but the Python C Extension interface allows PAT to be used through the 
CPython interpreter.

## How do I use it?
PAT is currently under very active development, and the API WILL BREAK on occasion; however, one 
can currently play audio files.
```python
import pat

# Queue an Audio File for Playback
pat.play('deerdance.m4a')

# Stop Playback
pat.stop()
```

## Licensing Considerations
You must link against an LGPL version of ffmpeg if you wish to bound by the terms of the LGPL.

