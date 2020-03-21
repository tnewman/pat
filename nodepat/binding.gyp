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
            "<!(pkg-config libavcodec --cflags-only-I | cut -c3-)",
            "<!(pkg-config libavdevice --cflags-only-I | cut -c3-)",
            "<!(pkg-config libavformat --cflags-only-I | cut -c3-)",
            "<!(pkg-config libavutil --cflags-only-I | cut -c3-)",
            "<!(pkg-config libswresample --cflags-only-I | cut -c3-)",
            "<!(pkg-config sdl2 --cflags-only-I | cut -c3-)"
          ],
          "libraries": [
            "<!(pkg-config libavcodec --libs)",
            "<!(pkg-config libavdevice --libs)",
            "<!(pkg-config libavformat --libs)",
            "<!(pkg-config libavutil --libs)",
            "<!(pkg-config libswresample --libs)",
            "<!(pkg-config sdl2 --libs)"
          ]
        }]
      ]
    }
  ]
}
