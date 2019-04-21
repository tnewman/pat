let assert = require('assert');
let nodepat = require('../dist/nodepat');

const VALID_AUDIO_PATH = __dirname + '/../src/libpat/test/test.mp3';
const MISSING_AUDIO_PATH = 'missing';
const INVALID_AUDIO_PATH = __dirname + '/../src/libpat/test/invalid.mp3';

describe('pat', function() {
    const TIMEOUT = 5000;

    it('should play an audio file', function() {
        return nodepat.play(VALID_AUDIO_PATH);
    }).timeout(TIMEOUT);

    it('should play an audio file synchronously', function() {
        nodepat.playSync(VALID_AUDIO_PATH);
    }).timeout(TIMEOUT);

    it('should reject a missing audio file', function() {
        return nodepat.play(MISSING_AUDIO_PATH).then(() => {
            assert.fail("missing audio should not play");
        }).catch((error) => {
            assert.deepStrictEqual(error.message,
                "Could not open the audio file. The audio file is inaccessible or does not exist.");
        });
    }).timeout(TIMEOUT);

    it('should reject a missing audio file synchronously', function() {
        try {
            nodepat.playSync(MISSING_AUDIO_PATH);
        } catch(error) {
            assert.deepStrictEqual(error.message,
                "Could not open the audio file. The audio file is inaccessible or does not exist.");
            return;
        }

        assert.fail("missing audio should not play");
    }).timeout(TIMEOUT);

    it('should reject an invalid audio file', function() {
        return nodepat.play(INVALID_AUDIO_PATH).then(() => {
            assert.fail("invalid audio should not play");
        }).catch((error) => {
            assert.deepStrictEqual(error.message,
                "Could not demux the audio file. The audio file format is unsupported or corrupted, or " +
                "the audio file does not contain an audio stream.");
        });
    }).timeout(TIMEOUT);

    it('should reject an invalid audio file synchronously', function() {
       try {
           nodepat.playSync(INVALID_AUDIO_PATH);
       }  catch(error) {
           assert.deepStrictEqual(error.message, "Could not demux the audio file. The audio file format is " +
               "unsupported or corrupted, or the audio file does not contain an audio stream.");
           return;
       }

       assert.fail("invalid audio should not play");
    }).timeout(TIMEOUT);

    it('should pause and resume an audio file', function() {
        let startTime = Date.now();

        let playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
            let elapsedTime = Date.now() - startTime;

            if(elapsedTime < 1500) {
                assert.fail("pause did not occur");
            }
        });

        nodepat.pause().then(() => {
            setTimeout(() => {
                nodepat.resume();
            }, 1000);
        });

        return playPromise;
    }).timeout(TIMEOUT);

    it('should pause and resume an audio file synchronously', async function() {
        let startTime = Date.now();

        let playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
            let elapsedTime = Date.now() - startTime;

            if(elapsedTime < 1500) {
                assert.fail("pause did not occur");
            }
        });

        // Setting a timeout is required to ensure pause is not called before the Node worker thread for play runs
        setTimeout(() => {
            nodepat.pauseSync();
            setTimeout(() => nodepat.resume(), 1000);
        }, 100);

        return playPromise;
    }).timeout(TIMEOUT);

    it('should skip an audio file', function() {
        let startTime = Date.now();

        let playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
            let elapsedTime = Date.now() - startTime;

            if(elapsedTime > 500) {
                assert.fail("skip did not occur");
            }
        });

        nodepat.skip();

        return playPromise;
    }).timeout(TIMEOUT);

    it('should skip an audio file synchronously', async function() {
        let startTime = Date.now();

        let playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
            let elapsedTime = Date.now() - startTime;

            if(elapsedTime > 500) {
                assert.fail("skip did not occur");
            }
        });

        // Setting a timeout is required to ensure pause is not called before the Node worker thread for play runs
        setTimeout(() => {
            nodepat.pauseSync();
            nodepat.skipSync();
        }, 100);

        return playPromise;
    }).timeout(TIMEOUT);
});
