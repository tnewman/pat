import assert from 'assert';
import path from 'path';
import * as nodepat from '../src/nodepat';

const VALID_AUDIO_PATH = path.join(__dirname, '../src/libpat/test/test.mp3');
const MISSING_AUDIO_PATH = 'missing';
const INVALID_AUDIO_PATH = path.join(__dirname, '../src/libpat/test/invalid.mp3');

describe('pat', () => {
  const TIMEOUT = 5000;

  it('should play an audio file', () => nodepat.play(VALID_AUDIO_PATH)).timeout(TIMEOUT);

  it('should play an audio file synchronously', () => {
    nodepat.playSync(VALID_AUDIO_PATH);
  }).timeout(TIMEOUT);

  it('should reject a missing audio file', () => nodepat.play(MISSING_AUDIO_PATH).then(() => {
    assert.fail('missing audio should not play');
  }).catch((error) => {
    assert.deepStrictEqual(error.message,
      'Could not open the audio file. The audio file is inaccessible or does not exist.');
  })).timeout(TIMEOUT);

  it('should reject a missing audio file synchronously', () => {
    try {
      nodepat.playSync(MISSING_AUDIO_PATH);
    } catch (error) {
      assert.deepStrictEqual(error.message,
        'Could not open the audio file. The audio file is inaccessible or does not exist.');
      return;
    }

    assert.fail('missing audio should not play');
  }).timeout(TIMEOUT);

  it('should reject an invalid audio file', () => nodepat.play(INVALID_AUDIO_PATH).then(() => {
    assert.fail('invalid audio should not play');
  }).catch((error) => {
    assert.deepStrictEqual(error.message,
      'Could not demux the audio file. The audio file format is unsupported or corrupted, or '
        + 'the audio file does not contain an audio stream.');
  })).timeout(TIMEOUT);

  it('should reject an invalid audio file synchronously', () => {
    try {
      nodepat.playSync(INVALID_AUDIO_PATH);
    } catch (error) {
      assert.deepStrictEqual(error.message, 'Could not demux the audio file. The audio file format is '
        + 'unsupported or corrupted, or the audio file does not contain an audio stream.');
      return;
    }

    assert.fail('invalid audio should not play');
  }).timeout(TIMEOUT);

  it('should pause and resume an audio file', () => {
    const startTime = Date.now();

    const playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
      const elapsedTime = Date.now() - startTime;

      if (elapsedTime < 1500) {
        assert.fail('pause did not occur');
      }
    });

    nodepat.pause().then(() => {
      setTimeout(() => {
        nodepat.resume();
      }, 1000);
    });

    return playPromise;
  }).timeout(TIMEOUT);

  it('should pause and resume an audio file synchronously', async () => {
    const startTime = Date.now();

    const playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
      const elapsedTime = Date.now() - startTime;

      if (elapsedTime < 1500) {
        assert.fail('pause did not occur');
      }
    });

    // Setting a timeout is required to ensure pause is not called
    // before the Node worker thread for play runs
    setTimeout(() => {
      nodepat.pauseSync();
      setTimeout(() => nodepat.resume(), 1000);
    }, 100);

    return playPromise;
  }).timeout(TIMEOUT);

  it('should skip an audio file', () => {
    const startTime = Date.now();

    const playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
      const elapsedTime = Date.now() - startTime;

      if (elapsedTime > 500) {
        assert.fail('skip did not occur');
      }
    });

    nodepat.skip();

    return playPromise;
  }).timeout(TIMEOUT);

  it('should skip an audio file synchronously', async () => {
    const startTime = Date.now();

    const playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
      const elapsedTime = Date.now() - startTime;

      if (elapsedTime > 500) {
        assert.fail('skip did not occur');
      }
    });

    // Setting a timeout is required to ensure pause is not
    // called before the Node worker thread for play runs
    setTimeout(() => {
      nodepat.pauseSync();
      nodepat.skipSync();
    }, 100);

    return playPromise;
  }).timeout(TIMEOUT);
});
