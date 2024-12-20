pub const MAX_TERMS: u32 = 10000;
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
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct TermData {
    pub term: *mut ::std::os::raw::c_char,
    pub tfidf_values: *mut f64,
    pub idf: f64,
}
#[allow(clippy::unnecessary_operation, clippy::identity_op)]
const _: () = {
    ["Size of TermData"][::std::mem::size_of::<TermData>() - 24usize];
    ["Alignment of TermData"][::std::mem::align_of::<TermData>() - 8usize];
    ["Offset of field: TermData::term"][::std::mem::offset_of!(TermData, term) - 0usize];
    ["Offset of field: TermData::tfidf_values"]
        [::std::mem::offset_of!(TermData, tfidf_values) - 8usize];
    ["Offset of field: TermData::idf"][::std::mem::offset_of!(TermData, idf) - 16usize];
};
unsafe extern "C" {
    pub fn tfidf_initialize(engine: *mut TFIDFEngine);
}
unsafe extern "C" {
    pub fn tfidf_load_documents(engine: *mut TFIDFEngine, directory: *const ::std::os::raw::c_char);
}
unsafe extern "C" {
    pub fn tfidf_calculate(engine: *mut TFIDFEngine);
}
unsafe extern "C" {
    pub fn tfidf_search(engine: *mut TFIDFEngine, query: *const ::std::os::raw::c_char);
}
unsafe extern "C" {
    pub fn tfidf_free(engine: *mut TFIDFEngine);
}
