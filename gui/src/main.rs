extern crate libc;

mod bindings;
use bindings::*;

use std::ffi::CString;

fn main() {
    unsafe {
        let mut engine: TFIDFEngine = std::mem::zeroed();

        let query = CString::new("Alice's").unwrap();

        tfidf_search(&mut engine, query.as_ptr());
    }
}
