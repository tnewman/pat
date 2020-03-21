import os
import pypat
import time
from concurrent import futures

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))
VALID_AUDIO_PATH = CURRENT_DIRECTORY + '/../pypat/libpat/test/test.ogg'
MISSING_AUDIO_PATH = 'missing'
INVALID_AUDIO_PATH = CURRENT_DIRECTORY + '/../pypat/libpat/test/invalid.ogg'


def test_play_audio_file():
    pypat.play(VALID_AUDIO_PATH)


def test_play_missing_audio_file_error():
    try:
        pypat.play(MISSING_AUDIO_PATH)
        raise Exception('missing audio should not play')
    except pypat.PATException as error:
        assert 'Could not open the audio file. The audio file is inaccessible or does not exist.' == str(error)


def test_play_invalid_audio_file_error():
    try:
        pypat.play(INVALID_AUDIO_PATH)
        raise Exception('invalid audio should not play')
    except pypat.PATException as error:
        assert 'Could not demux the audio file. The audio file format is unsupported or corrupted, or '\
               'the audio file does not contain an audio stream.' == str(error)


def test_pause_resume_audio_file():
    def play_thread():
        start = time.time()

        pypat.play(VALID_AUDIO_PATH)

        end = time.time()

        elapsed = end - start

        if elapsed < 1.5:
            raise Exception('pause did not occur')

    pool = futures.ThreadPoolExecutor(max_workers=1)
    future = pool.submit(play_thread)

    # If this thread does not sleep, pause will run before the playback thread
    time.sleep(0.750)

    pypat.pause()

    time.sleep(0.750)

    pypat.resume()

    future.result()


def test_skip_audio_file():
    def play_thread():
        start = time.time()

        pypat.play(VALID_AUDIO_PATH)

        end = time.time()

        elapsed = end - start

        if elapsed > 1:
            raise Exception('skip did not occur')

    pool = futures.ThreadPoolExecutor(max_workers=1)
    future = pool.submit(play_thread)

    # If this thread does not sleep, skip will run before the playback thread
    time.sleep(0.250)

    pypat.skip()

    future.result()
