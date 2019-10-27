const {
  _play,
  _skip,
  _pause,
  _resume,
} = require('../build/Release/_nodepat.node');

export async function play(audioPath: string): Promise<void> {
  return _play(audioPath);
}

export async function skip(): Promise<void> {
  _skip();
}

export async function pause(): Promise<void> {
  _pause();
}

export async function resume(): Promise<void> {
  _resume();
}

(async () => {
  await play('C:/Users/tnewm/Downloads/test.mpga');
})();
