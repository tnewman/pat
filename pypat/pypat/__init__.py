from ctypes import *
from enum import IntEnum, unique
import atexit
import os
import signal
import sys


def play(audio_path: str):
    """
    Play an audio file.
    :param audio_path: The path to the audio file to play. This can be a local file or remote file (http:// or https://)
    :raises PATException: Raised when PAT cannot play an audio file.
    """
    pat_error = _libpat.pat_play(_pat, c_char_p(audio_path.encode('ascii')))
    _check_error(audio_path, pat_error)


def skip():
    """
    Skip playback of the current audio file. This method does nothing if there is not an audio file playing.
    :raises PATException: Raised when PAT cannot skip playback.
    """
    pat_error = _libpat.pat_skip(_pat)

    if pat_error != _PATError.PAT_SUCCESS:
        raise PATException('Could not skip playback.\n{}'.format(_error_to_str(pat_error)))


def pause():
    """
    Pause audio playback. This method does nothing if audio playback is already paused.
    :raises PATException: Raised when PAT cannot pause playback.
    """
    pat_error = _libpat.pat_pause(_pat)

    if pat_error != _PATError.PAT_SUCCESS:
        raise PATException('Could not pause playback.\n{}'.format(_error_to_str(pat_error)))


def resume():
    """
    Resume audio playback. This method does nothing if audio playback is already resumed.
    :raises PATException: Raised when PAT cannot resume playback.
    """
    pat_error = _libpat.pat_resume(_pat)

    if pat_error != _PATError.PAT_SUCCESS:
        raise PATException('Could not resume playback.\n{}'.format(_error_to_str(pat_error)))


@unique
class _PATError(IntEnum):
    PAT_SUCCESS = 0
    PAT_AUDIO_DEVICE_ERROR = 1
    PAT_DEMUX_ERROR = 2
    PAT_DECODE_ERROR = 3
    PAT_FILE_OPEN_ERROR = 4
    PAT_INTERRUPTED_ERROR = 5
    PAT_MEMORY_ERROR = 6
    PAT_RESAMPLE_ERROR = 7
    PAT_TERMINATED_ERROR = 8
    PAT_UNKNOWN_ERROR = 9


class PATException(Exception):
    pass


def _get_libpat_path():
    if sys.platform == 'darwin':
        _shared_lib_suffix = 'dylib'
    elif sys.platform == 'win32':
        _shared_lib_suffix = 'dll'
    else:
        _shared_lib_suffix = 'so'

    module_folder = os.path.dirname(__file__)

    libpat_paths = [f'{module_folder}/libpat/build/bin/libpat.{_shared_lib_suffix}',
                    f'{module_folder}/libpat.{_shared_lib_suffix}']
    
    for libpat_path in libpat_paths:
        if os.path.exists(libpat_path):
            return libpat_path

    raise PATException('Failed to load libpat.')


def _load_libpat():
    libpat_path = _get_libpat_path()

    libpat = cdll.LoadLibrary(libpat_path)

    libpat.pat_error_to_string.restype = c_char_p

    return libpat


def _pat_open():
    pat = c_void_p()
    pat_error = _libpat.pat_open(byref(pat))

    if pat_error != 0:
        raise PATException('Error initializing PAT.\n{}'.format(_error_to_str(pat_error)))

    return pat


def _pat_close():
    _libpat.pat_close(_pat)


def _check_error(audio_path, pat_error):
    if pat_error == _PATError.PAT_INTERRUPTED_ERROR:
        os.kill(os.getpid(), signal.SIGINT)
    elif pat_error == _PATError.PAT_TERMINATED_ERROR:
        os.kill(os.getpid(), signal.SIGTERM)
    elif pat_error != _PATError.PAT_SUCCESS:
        raise PATException('Could not play {}.\n{}'.format(audio_path, _error_to_str(pat_error)))


def _error_to_str(pat_error: int):
    return _libpat.pat_error_to_string(pat_error).decode('ascii')


_libpat = _load_libpat()
_pat = _pat_open()
atexit.register(_pat_close)
