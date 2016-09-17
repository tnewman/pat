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

## License
PAT is licensed under the LGPLv2.1 as it requires ffmpeg, which is LGPLv2.1 licensed. Note that if 
you link against a GPL licensed version of ffmpeg, you must license your program under the GPL. 
PAT links against its own LGPLv2.1 version of ffmpeg by default to ensure that PAT can be used 
under the terms of the LGPLv2.1 as the OS version of ffmpeg may be GPL licensed.

## Current limitations
* Only works with audio files that have a 44100 sample rate. This was done to simplify the resampling 
  code.
