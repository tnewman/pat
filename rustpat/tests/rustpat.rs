extern crate rustpat;

#[test]
fn test_play_audio() {
    let pat = rustpat::PAT::new().unwrap();

    let test_audio = format!("{}/src/libpat/test/test.ogg", env!("CARGO_MANIFEST_DIR"));

    pat.play(&test_audio).unwrap();
}

#[test]
#[should_panic]
fn test_play_invalid_audio() {
    let pat = rustpat::PAT::new().unwrap();

    pat.play("invalid").unwrap();
}