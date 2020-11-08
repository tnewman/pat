# rustpat
rustpat is a Rust library that makes playing audio simple. rustpat uses FFmpeg to provide 
robust support for virtually any audio format and SDL2 to play back audio.

## Prerequisites
Install [Prerequisites](https://github.com/tnewman/pat#Prerequisites) before installing.

## Example
```rust
extern crate rustpat;

fn main() {
    let pat = rustpat::PAT::new().unwrap();

    // Play a file
    pat.play('amerika.mpga');

    // Skip the current file
    pat.skip();

    // Pause audio playback
    pat.pause();

    // Resume audio playback
    pat.resume();
}
```
