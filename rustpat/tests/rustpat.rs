use std::{sync::Arc, thread, time::{Duration, Instant, SystemTime}};

extern crate rustpat;

#[test]
fn test_play_audio() {
    let pat = rustpat::PAT::new().unwrap();

    let test_audio = format!("file://{}/tests/test.ogg", env!("CARGO_MANIFEST_DIR"));

    pat.play(&test_audio).unwrap();
}

#[test]
#[should_panic]
fn test_play_invalid_audio_file() {
    let pat = rustpat::PAT::new().unwrap();

    let test_audio = format!("file://{}/tests/invalid.ogg", env!("CARGO_MANIFEST_DIR"));

    pat.play(&test_audio).unwrap();
}

#[test]
#[should_panic]
fn test_play_missing_audio_file() {
    let pat = rustpat::PAT::new().unwrap();

    pat.play("invalid").unwrap();
}

#[test]
fn test_pause_resume_audio_file() {
    let pat = Arc::from(rustpat::PAT::new().unwrap());

    let thread_pat = pat.clone();

    let play_thread = thread::spawn(move || {
        let start = Instant::now();

        let test_audio = format!("file://{}/tests/test.ogg", env!("CARGO_MANIFEST_DIR"));

        thread_pat.play(&test_audio).unwrap();

        let elapsed = start.elapsed();

        if elapsed.le(&Duration::from_millis(1500)) {
            panic!("pause did not occur")
        }
    });

    // If this thread does not sleep, skip will run before the playback thread
    thread::sleep(Duration::from_millis(750));

    pat.pause().unwrap();

    thread::sleep(Duration::from_millis(750));

    pat.resume().unwrap();

    play_thread.join().unwrap();
}

#[test]
fn test_skip_audio_file() {
    let pat = Arc::from(rustpat::PAT::new().unwrap());

    let thread_pat = pat.clone();

    let play_thread = thread::spawn(move || {
        let start = Instant::now();

        let test_audio = format!("file://{}/tests/test.ogg", env!("CARGO_MANIFEST_DIR"));

        thread_pat.play(&test_audio).unwrap();

        let elapsed = start.elapsed();

        if elapsed.ge(&Duration::from_secs(1)) {
            panic!("skip did not occur")
        }
    });

    // If this thread does not sleep, skip will run before the playback thread
    thread::sleep(Duration::from_millis(250));

    pat.skip().unwrap();

    play_thread.join().unwrap();
}