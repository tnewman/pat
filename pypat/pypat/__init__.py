from ctypes import *
from ctypes.util import find_library
from enum import IntEnum, unique
import atexit
import os
import signal
import sys


def play(audio_path: str):
    """
    Play an audio file with PAT.
    :param audio_path: The path to the audio file to play. This can be a local file or remote file (http:// or https://)
    :raises PATException: Raised when PAT cannot play an audio file.
    """
    pat_error = _libpat.pat_play(_pat, c_char_p(audio_path.encode('ascii')))

    if pat_error == _PATError.PAT_SUCCESS:
        return
    elif pat_error == _PATError.PAT_INTERRUPTED_ERROR:
        os.kill(os.getpid(), signal.SIGINT)
    elif pat_error == _PATError.PAT_TERMINATED_ERROR:
        os.kill(os.getpid(), signal.SIGTERM)
        return
    else:
        raise PATException('Could not play {}.\n{}'.format(audio_path, _error_to_str(pat_error)))


def skip():
    """
    Skip playback of the current audio file. This method does nothing if there is not an audio file playing.
    :raises PATException: Raised when PAT cannot skip playback.
    """
    pat_error = _libpat.pat_skip(_pat)

    if pat_error != _PATError.PAT_SUCCESS:
        raise PATException('Could not skip playback.\n{}'.format(_error_to_str(pat_error)))


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
    if sys.platform == 'linux':
        shared_lib_suffix = 'so'
    else:
        raise PATException('Unsupported OS. PAT only supports Linux.')

    libpat_paths = [os.path.abspath(f'{__file__}/../../../libpat/build/libpat/libpat.{shared_lib_suffix}'),
                    os.path.abspath(f'{__file__}/../libpat.{shared_lib_suffix}')]
    
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


def _error_to_str(pat_error: int):
    return _libpat.pat_error_to_string(pat_error).decode('ascii')


_libpat = _load_libpat()
_pat = _pat_open()
atexit.register(_pat_close)
