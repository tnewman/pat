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
      "libraries": [
        "-lavcodec",
        "-lavformat",
        "-lavutil",
        "-lswresample",
        "-lSDL2"
      ]
    }
  ]
}
