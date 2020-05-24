// eslint-disable-next-line @typescript-eslint/no-var-requires
const {_play, _skip, _pause, _resume,} = require('../build/Release/_nodepat.node');

/**
 * Play an audio file.
 * 
 * @param audioPath The path to the audio file to play. This can be a local file or remote file (http:// or https://).
 * @throws {@link Error} Thrown when PAT cannot play an audio file.
 */
export async function play(audioPath: string): Promise<void> {
  return _play(audioPath);
}

/**
 * Play an audio file.
 * 
 * @param audioPath The path to the audio file to play. This can be a local file or remote file (http:// or https://).
 * @throws {@link Error} Thrown when PAT cannot play an audio file.
 */
export async function skip(): Promise<void> {
  _skip();
}

/**
 * Skip playback of the current audio file. This method does nothing if there is not an audio file playing.
 * 
 * @throws {@link Error} Thrown when PAT cannot skip playback.
 */
export async function pause(): Promise<void> {
  _pause();
}

/**
 * Resume audio playback. This method does nothing if audio playback is already resumed.
 * 
 * @throws {@link Error} Thrown when PAT cannot resume playback.
 */
export async function resume(): Promise<void> {
  _resume();
}

export default {
  play,
  skip,
  pause,
  resume
}
