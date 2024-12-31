#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct TFIDFEngine {
    pub documents: *mut *mut ::std::os::raw::c_char,
    pub num_documents: ::std::os::raw::c_int,
}
#[allow(clippy::unnecessary_operation, clippy::identity_op)]
const _: () = {
    ["Size of TFIDFEngine"][::std::mem::size_of::<TFIDFEngine>() - 16usize];
    ["Alignment of TFIDFEngine"][::std::mem::align_of::<TFIDFEngine>() - 8usize];
    ["Offset of field: TFIDFEngine::documents"]
        [::std::mem::offset_of!(TFIDFEngine, documents) - 0usize];
    ["Offset of field: TFIDFEngine::num_documents"]
        [::std::mem::offset_of!(TFIDFEngine, num_documents) - 8usize];
};

unsafe extern "C" {
    pub fn tfidf_search(engine: *mut TFIDFEngine, query: *const ::std::os::raw::c_char);
}

