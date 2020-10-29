use cc;

fn main() {
    cc::Build::new()
        .include("src/libpat/libpat/include")
        .include("/usr/include/SDL2")
        .include("/usr/local/include/SDL2")
        .file("src/libpat/libpat/src/pat_audio_device.c")
        .file("src/libpat/libpat/src/pat_decode.c")
        .file("src/libpat/libpat/src/pat_error.c")
        .file("src/libpat/libpat/src/pat_ring_buffer.c")
        .file("src/libpat/libpat/src/pat.c")
        .compile("pat");
}
