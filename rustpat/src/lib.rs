#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[allow(non_upper_case_globals)]
mod bindings;

use std::ffi::CString;

pub struct PAT {
    pat_ptr: *mut bindings::PAT,
}

impl PAT {
    pub fn new() -> Result<PAT, PATError> {
        let mut pat_ptr: *mut bindings::PAT = std::ptr::null_mut();

        let result;

        unsafe {
            result = PAT::from_pat_result(bindings::pat_open(&mut pat_ptr));
        }

        match result {
            Ok(_) => Ok(PAT { pat_ptr }),
            Err(error) => Err(error),
        }
    }

    pub fn play(&self, pat_audio_path: &str) -> Result<(), PATError> {
        let pat_audio_path = match CString::new(pat_audio_path) {
            Ok(pat_audio_path) => pat_audio_path,
            Err(_) => return Err(PATError::UnknownError),
        };

        let result;

        unsafe {
            result = bindings::pat_play(self.pat_ptr, pat_audio_path.as_ptr());
        }

        PAT::from_pat_result(result)
    }

    pub fn skip(&self) -> Result<(), PATError> {
        let result;

        unsafe {
            result = bindings::pat_skip(self.pat_ptr);
        }

        PAT::from_pat_result(result)
    }

    pub fn pause(&self) -> Result<(), PATError> {
        let result;

        unsafe {
            result = bindings::pat_pause(self.pat_ptr);
        }

        PAT::from_pat_result(result)
    }

    pub fn resume(&self) -> Result<(), PATError> {
        let result;

        unsafe {
            result = bindings::pat_resume(self.pat_ptr);
        }

        PAT::from_pat_result(result)
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

impl PAT {
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

impl Drop for PAT {
    fn drop(&mut self) {
        unsafe {
            bindings::pat_close(self.pat_ptr);
        }
    }
}

unsafe impl Send for PAT {}
unsafe impl Sync for PAT {}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        let _pat = crate::PAT::new();

        std::thread::spawn(move || {});
    }
}
