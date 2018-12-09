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
    _check_error(_libpat.pat_play(_pat, c_char_p(audio_path.encode('ascii'))))


def skip():
    """
    Skip playback of the current audio file. This method does nothing if there is not an audio file playing.
    :raises PATException: Raised when PAT cannot skip playback.
    """
    _check_error(_libpat.pat_skip(_pat))


def pause():
    """
    Pause audio playback. This method does nothing if audio playback is already paused.
    :raises PATException: Raised when PAT cannot pause playback.
    """
    _check_error(_libpat.pat_pause(_pat))


def resume():
    """
    Resume audio playback. This method does nothing if audio playback is already resumed.
    :raises PATException: Raised when PAT cannot resume playback.
    """
    _check_error(_libpat.pat_resume(_pat))


class PATException(Exception):
    pass


_PAT_SUCCESS = 0


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
    _check_error(_libpat.pat_open(byref(pat)))
    return pat


def _pat_close():
    _libpat.pat_close(_pat)


def _check_error(pat_error):
    if pat_error != _PAT_SUCCESS:
        raise PATException(_libpat.pat_error_to_string(pat_error).decode('ascii'))


_libpat = _load_libpat()
_pat = _pat_open()
atexit.register(_pat_close)
