# PAT Audio Technician (PAT)
PAT is a Python library used to play audio files. If you have audio files and want to play them using 
Python, PAT the Audio Technician is at your service.

## How does it work?
PAT uses ffmpeg for audio decoding and resampling. SDL is used to play audio using the sound card. 
Everything is written in C, but the Python C Extension interface allows PAT to be used through the 
CPython interpreter.

## How do I use it?
PAT is currently under very active development; however, one can currently play songs synchronously.
```python
import pat
pat.play('deerdance.m4a')
```

## Current limitations
* Only works with audio files that have a 44100 sample rate. This was done to simplify the resampling 
  code.
