typedef struct PATAudioDevice PATAudioDevice;

PATAudioDevice* pat_open_audio_device();

void pat_free_audio_device(PATAudioDevice* pat_audio_device);
