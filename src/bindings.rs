#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/peaty.rs"));

impl Drop for SparseGraph {
    fn drop(&mut self) {
        unsafe { self.destruct() };
    }
}

impl Clone for SparseGraph {
    fn clone(&self) -> Self {
        unsafe { Self::new1(self) }
    }
}

impl Drop for Result {
    fn drop(&mut self) {
        unsafe { self.destruct() };
    }
}
