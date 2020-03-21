{
  "targets": [
    {
      "target_name": "_nodepat",
      "include_dirs" : [
        "src/libpat/libpat/include",
        "src/libpat/libpat/src"
      ],
      "sources": [
        "src/_nodepat.c",
        "src/libpat/libpat/src/pat.c",
        "src/libpat/libpat/src/pat_audio_device.c",
        "src/libpat/libpat/src/pat_decode.c",
        "src/libpat/libpat/src/pat_error.c",
        "src/libpat/libpat/src/pat_ring_buffer.c"
      ],
      "conditions": [
        ["OS == 'win'", {
          "variables": {
            'ffmpeg-root%': 'C:/ffmpeg',
            'sdl2-root%': 'C:/SDL2'
          },
          "include_dirs": [
            "<(ffmpeg-root)/include",
            "<(sdl2-root)/include/SDL2"
          ],
          "libraries": [
            "<(ffmpeg-root)/lib/avcodec.lib",
            "<(ffmpeg-root)/lib/avdevice.lib",
            "<(ffmpeg-root)/lib/avformat.lib",
            "<(ffmpeg-root)/lib/avutil.lib",
            "<(ffmpeg-root)/lib/swresample.lib",
            "<(sdl2-root)/lib/x64/SDL2.lib",
            "<(sdl2-root)/lib/x86/SDL2.lib"
          ]
        }, {
          "include_dirs": [
            "/usr/include/SDL2"
          ],
          "libraries": [
            "-lavcodec",
            "-lavdevice",
            "-lavformat",
            "-lavutil",
            "-lswresample",
            "-lSDL2"
          ]
        }]
      ]
    }
  ]
}
