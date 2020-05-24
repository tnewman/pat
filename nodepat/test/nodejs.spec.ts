import assert from 'assert';
import path from 'path';
import nodepat from '../src/nodepat';
import { play, pause, resume, skip } from '../src/nodepat';

const VALID_AUDIO_PATH = path.join(__dirname, '../src/libpat/test/test.ogg');
const MISSING_AUDIO_PATH = 'missing';
const INVALID_AUDIO_PATH = path.join(__dirname, '../src/libpat/test/invalid.ogg');

describe('pat', () => {
  const TIMEOUT = 5000;

  it('should play an audio file', () => play(VALID_AUDIO_PATH)).timeout(TIMEOUT);

  it('should reject a missing audio file', () => play(MISSING_AUDIO_PATH).then(() => {
    assert.fail('missing audio should not play');
  }).catch((error) => {
    assert.deepStrictEqual(error.message,
      'Could not open the audio file. The audio file is inaccessible or does not exist.');
  })).timeout(TIMEOUT);

  it('should reject an invalid audio file', () => play(INVALID_AUDIO_PATH).then(() => {
    assert.fail('invalid audio should not play');
  }).catch((error) => {
    assert.deepStrictEqual(error.message,
      'Could not demux the audio file. The audio file format is unsupported or corrupted, or '
        + 'the audio file does not contain an audio stream.');
  })).timeout(TIMEOUT);

  it('should pause and resume an audio file', () => {
    const startTime = Date.now();

    const playPromise = play(VALID_AUDIO_PATH).then(() => {
      const elapsedTime = Date.now() - startTime;

      if (elapsedTime < 1500) {
        assert.fail('pause did not occur');
      }
    });

    pause().then(() => {
      setTimeout(() => {
        resume();
      }, 1000);
    });

    return playPromise;
  }).timeout(TIMEOUT);

  it('should skip an audio file', () => {
    const startTime = Date.now();

    const playPromise = play(VALID_AUDIO_PATH).then(() => {
      const elapsedTime = Date.now() - startTime;

      if (elapsedTime > 500) {
        assert.fail('skip did not occur');
      }
    });

    skip();

    return playPromise;
  }).timeout(TIMEOUT);

  it('should provide default exports', () => {
    assert(nodepat.play);
    assert(nodepat.pause);
    assert(nodepat.resume);
    assert(nodepat.skip);
  });
});
