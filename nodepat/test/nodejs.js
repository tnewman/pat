let assert = require('assert');
let nodepat = require('../src/nodepat');

const VALID_AUDIO_PATH = __dirname + '/../src/libpat/test/test.mp3';
const MISSING_AUDIO_PATH = 'missing';
const INVALID_AUDIO_PATH = __dirname + '/../src/libpat/test/invalid.mp3';

describe('pat', () => {
    it('should play an audio file', function() {
        return nodepat.play(VALID_AUDIO_PATH);
    });

    it('should play an audio file synchronously', function() {
        nodepat.playSync(VALID_AUDIO_PATH);
    });

    it('should reject a missing audio file', function() {
        return nodepat.play(MISSING_AUDIO_PATH).then(() => {
            assert.fail("missing audio should not play");
        }).catch((error) => {
            assert.deepStrictEqual(error.message,
                "Could not open the audio file. The audio file is inaccessible or does not exist.");
        });
    });

    it('should reject a missing audio file synchronously', function() {
        try {
            nodepat.playSync(MISSING_AUDIO_PATH);
        } catch(error) {
            assert.deepStrictEqual(error.message,
                "Could not open the audio file. The audio file is inaccessible or does not exist.");
            return;
        }

        assert.fail("missing audio should not play");
    });

    it('should reject an invalid audio file', function() {
        return nodepat.play(INVALID_AUDIO_PATH).then(() => {
            assert.fail("invalid audio should not play");
        }).catch((error) => {
            assert.deepStrictEqual(error.message,
                "Could not demux the audio file. The audio file format is unsupported or corrupted, or " +
                "the audio file does not contain an audio stream.");
        });
    });

    it('should reject an invalid audio file synchronously', function() {
       try {
           nodepat.playSync(INVALID_AUDIO_PATH);
       }  catch(error) {
           assert.deepStrictEqual(error.message, "Could not demux the audio file. The audio file format is " +
               "unsupported or corrupted, or the audio file does not contain an audio stream.");
           return;
       }

       assert.fail("invalid audio should not play");
    });

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
            }, 750);
        });

        return playPromise;
    });

    it('should pause and resume an audio file synchronously', async function() {
        let startTime = Date.now();

        let playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
            let elapsedTime = Date.now() - startTime;

            if(elapsedTime < 1500) {
                assert.fail("pause did not occur");
            }
        });

        nodepat.pauseSync();

        setTimeout(() => nodepat.resumeSync(), 750);

        return playPromise;
    });

    it('should skip an audio file', function() {
        let startTime = Date.now();

        let playPromise = nodepat.play(VALID_AUDIO_PATH).then(() => {
            let elapsedTime = Date.now() - startTime;

            if(elapsedTime > 1000) {
                assert.fail("skip did not occur");
            }
        });

        nodepat.skip();

        return playPromise;
    });
});
