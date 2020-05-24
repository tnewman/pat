# nodepat
nodepat is a NodeJS library that makes playing audio simple. nodepat uses FFmpeg to provide 
robust support for virtually any audio format and SDL2 to play back audio.

## Prerequisites
Install [Prerequisites](https://github.com/tnewman/pat#Prerequisites) before installing.

## Install
```bash
npm install nodepat
```

## Example
```typescript
import nodepat from 'nodepat';

// Alternatively
import { play, skip, pause, resume } from 'nodepat';

(async () => {
  // Play a file
  await nodepat.play('amerika.mpga');

  // Skip the current file
  await nodepat.skip();

  // Pause audio playback
  await nodepat.pause();

  // Resume audio playback
  await nodepat.resume();
})();
```
