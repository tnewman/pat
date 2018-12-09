# nodepat
nodepat is a NodeJS library that makes playing audio simple. nodepat uses FFmpeg to provide 
robust support for virtually any audio format and SDL2 to play back audio.

## Prerequisites
Install [Prerequisites](https://github.com/tnewman/pat/tree/nodejs-support#Prerequisites) before installing.

## Install
```bash
npm install nodepat
```

## Example
```javascript
"use strict";

const nodepat = require('nodepat');

// Play a file
nodepat.play('amerika.mpga')
    .then(() => console.log('Playback Finished'))
    .catch((error) => console.error(error));

// Skip the current file
nodepat.skip()
    .then(() => console.log('Skipped the Currently Playing File'))
    .catch((error) => console.error(error));

// Pause audio playback
nodepat.pause()
    .then(() => console.log('Paused Audio Playback'))
    .catch((error) => console.error(error));

// Resume audio playback
nodepat.resume()
    .then(() => console.log('Resumed Audio Playsback'))
    .catch((error) => console.error(error));
```
