use std::{ffi::CStr, os::raw::c_char, ptr};

use rustpat::{PAT, PATError};

#[no_mangle]
pub extern "C" fn pat_new() -> *mut PAT {
    match PAT::new() {
        Ok(pat) => Box::into_raw(Box::new(pat)),
        Err(_) => ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn pat_play(pat: *mut PAT, pat_audio_path: *const c_char) -> u32 {
    let pat = unsafe {
        assert!(!pat.is_null());
        &*pat
    };

    let pat_audio_path = unsafe {
        CStr::from_ptr(pat_audio_path)
    };

    let pat_audio_path = pat_audio_path.to_str();
    
    let pat_audio_path = match pat_audio_path {
        Ok(pat_audio_path) => pat_audio_path,
        Err(_) => return 1,
    };

    match pat.play(pat_audio_path) {
        Ok(_) => 0,
        Err(_) => 1,
    }
}

#[no_mangle]
pub extern "C" fn pat_skip(pat: *mut PAT) -> u32 {
    let pat = unsafe {
        assert!(!pat.is_null());
        &*pat
    };
    
    match pat.skip() {
        Ok(_) => 0,
        Err(_) => 1,
    }
}

#[no_mangle]
pub extern "C" fn pat_pause(pat: *mut PAT) -> u32 {
    let pat = unsafe {
        assert!(!pat.is_null());
        &*pat
    };

    match pat.pause() {
        Ok(_) => 0,
        Err(_) => 1,
    }
}

#[no_mangle]
pub extern "C" fn pat_resume(pat: *mut PAT) -> u32 {
    let pat = unsafe {
        assert!(!pat.is_null());
        &*pat
    };

    match pat.resume() {
        Ok(_) => 0,
        Err(_) => 1,
    }
}

#[no_mangle]
pub extern "C" fn pat_free(pat: *mut PAT) {
    if pat.is_null() {
        return;
    }

    unsafe {
        Box::from_raw(pat);
    }
}
