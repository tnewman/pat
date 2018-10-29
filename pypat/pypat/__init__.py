from ctypes import *
from ctypes.util import find_library
import atexit
import os
import sys


def play(audio_path: str):
    """
    Play an audio file with PAT.
    :param audio_path: The path to the audio file to play. This can be a local file or remote file (http:// or https://)
    :raises PATException: Raised when PAT cannot play an audio file.
    """
    pat_error = _libpat.pat_play(_pat, c_char_p(audio_path.encode('ascii')))

    if pat_error != 0:
        raise PATException('Could not play {}.\n{}'.format(audio_path, _error_to_str(pat_error)))


class PATException(Exception):
    pass


def _load_libpat():
    if sys.platform != 'linux':
        raise PATException('Unsupported OS. PAT only supports Linux.')

    library = find_library('pat')

    if library is None:
        libpat_path = os.path.abspath(__file__ + '/../../../libpat/build/libpat/libpat.so')

        if not os.path.exists(libpat_path):
            raise PATException('Failed to load libpat.')
    else:
        libpat_path = library

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
