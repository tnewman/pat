import pypat._pypat


def play(audio_path: str):
    """
    Play an audio file.
    :param audio_path: The path to the audio file to play. This can be a local file or remote file (http:// or https://)
    :raises PATException: Raised when PAT cannot play an audio file.
    """
    _pypat.play(audio_path)


def skip():
    """
    Skip playback of the current audio file. This method does nothing if there is not an audio file playing.
    :raises PATException: Raised when PAT cannot skip playback.
    """
    _pypat.skip()


def pause():
    """
    Pause audio playback. This method does nothing if audio playback is already paused.
    :raises PATException: Raised when PAT cannot pause playback.
    """
    _pypat.pause()


def resume():
    """
    Resume audio playback. This method does nothing if audio playback is already resumed.
    :raises PATException: Raised when PAT cannot resume playback.
    """
    _pypat.resume()
