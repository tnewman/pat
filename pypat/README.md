# pypat
pypat is a Python library that makes playing audio simple. pypat uses FFmpeg to provide 
robust support for virtually any audio format and SDL2 to play back audio.

## Prerequisites
Install [Prerequisites](https://github.com/tnewman/pat#Prerequisites) before installing.

## Install
```bash
pip3 install pypat
```

## Example
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
