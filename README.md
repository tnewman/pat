# PAT Audio Technician (PAT)
PAT is a Python library used to play audio files. If you have audio files and want to play them using 
Python, PAT the Audio Technician is at your service.

## How does it work?
PAT uses ffmpeg for audio decoding and resampling. SDL is used to play audio using the sound card. 
Everything is written in C, but the Python C Extension interface allows PAT to be used through the 
CPython interpreter.

## How do I use it?
import pat

# Play an Audio File
pat.play('deerdance.m4a')

# Stop Playback
pat.stop()
```

## License
PAT is licensed under the LGPLv2.1 or later as it requires ffmpeg. Note that you must link against 
an LGPL version of ffmpeg if you wish to bound by the terms of the LGPL.

