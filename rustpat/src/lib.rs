#[allow(dead_code)]
#[allow(non_camel_case_types)]
#[allow(non_upper_case_globals)]
mod bindings;

struct PAT {
    pat_ptr: *mut bindings::PAT
}

impl PAT {
    fn new() -> PAT {
        let mut pat_ptr: *mut bindings::PAT = std::ptr::null_mut();

        unsafe {
            bindings::pat_open(&mut pat_ptr);
        }

        PAT {
            pat_ptr: pat_ptr
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

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        let _pat = crate::PAT::new();
    }
}
