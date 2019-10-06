import _pypat


class PATException(Exception):
    """
    Thrown when an error occurs when using PAT.
    """
    pass


def _convert_exception(func):
    def wrapper(*args, **kwargs):
        try:
            func(*args, **kwargs)
        except Exception as exception:
            raise PATException(exception)
    return wrapper


@_convert_exception
def play(audio_path: str):
    """
    Play an audio file.
    :param audio_path: The path to the audio file to play. This can be a local file or remote file (http:// or https://)
    :raises PATException: Raised when PAT cannot play an audio file.
    """
    _pypat.play(audio_path)


@_convert_exception
def skip():
    """
    Skip playback of the current audio file. This method does nothing if there is not an audio file playing.
    :raises PATException: Raised when PAT cannot skip playback.
    """
    _pypat.skip()


@_convert_exception
def pause():
    """
    Pause audio playback. This method does nothing if audio playback is already paused.
    :raises PATException: Raised when PAT cannot pause playback.
    """
    _pypat.pause()


@_convert_exception
def resume():
    """
    Resume audio playback. This method does nothing if audio playback is already resumed.
    :raises PATException: Raised when PAT cannot resume playback.
    """
    _pypat.resume()
