use std::ffi::CString;
use std::sync::Once;

#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[allow(non_upper_case_globals)]
mod bindings;

pub struct PAT {}

static INIT: Once = Once::new();
static mut INIT_RESULT: Result<(), PATError> = Err(PATError::UnknownError);

impl PAT {
    pub fn new() -> Result<PAT, PATError> {
        unsafe {
            INIT.call_once(|| {
                let result = PATError::from_pat_result(bindings::pat_init());
    
                match result {
                    Ok(_) => INIT_RESULT = Ok(()),
                    Err(error) => INIT_RESULT = Err(error)
                }
            });

            match INIT_RESULT {
                Ok(_) => Ok(PAT {}),
                Err(error) => Err(error.clone())
            }
        }
    }

    pub fn play(&self, pat_audio_path: &str) -> Result<(), PATError> {
        let pat_audio_path = match CString::new(pat_audio_path) {
            Ok(pat_audio_path) => pat_audio_path,
            Err(_) => return Err(PATError::UnknownError),
        };

        let result;

        unsafe {
            result = bindings::pat_play(pat_audio_path.as_ptr());
        }

        PATError::from_pat_result(result)
    }

    pub fn skip(&self) -> Result<(), PATError> {
        let result;

        unsafe {
            result = bindings::pat_skip();
        }

        PATError::from_pat_result(result)
    }

    pub fn pause(&self) -> Result<(), PATError> {
        let result;

        unsafe {
            result = bindings::pat_pause();
        }

        PATError::from_pat_result(result)
    }

    pub fn resume(&self) -> Result<(), PATError> {
        let result;

        unsafe {
            result = bindings::pat_resume();
        }

        PATError::from_pat_result(result)
    }
}

#[derive(Clone, Copy, Debug, Eq, Hash, Ord, PartialEq, PartialOrd)]
pub enum PATError {
    AudioDeviceError,
    DemuxError,
    DecodeError,
    FileOpenError,
    InterruptedError,
    MemoryError,
    ResampleError,
    TerminatedError,
    UnknownError,
}

impl PATError {
    fn from_pat_result(pat_result: bindings::PATError) -> Result<(), PATError> {
        match pat_result {
            bindings::PATError_PAT_SUCCESS => Ok(()),
            bindings::PATError_PAT_AUDIO_DEVICE_ERROR => Err(PATError::AudioDeviceError),
            bindings::PATError_PAT_DEMUX_ERROR => Err(PATError::DemuxError),
            bindings::PATError_PAT_DECODE_ERROR => Err(PATError::DecodeError),
            bindings::PATError_PAT_FILE_OPEN_ERROR => Err(PATError::FileOpenError),
            bindings::PATError_PAT_INTERRUPTED_ERROR => Err(PATError::InterruptedError),
            bindings::PATError_PAT_MEMORY_ERROR => Err(PATError::MemoryError),
            bindings::PATError_PAT_RESAMPLE_ERROR => Err(PATError::ResampleError),
            bindings::PATError_PAT_TERMINATED_ERROR => Err(PATError::TerminatedError),
            _ => Err(PATError::UnknownError),
        }
    }
}
