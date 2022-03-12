pub static DEBUG: bool = cfg!(feature = "debug");

#[macro_export]
macro_rules! kassert {
    ($check:expr) => {
        if crate::kassert::DEBUG && !($check) {
            libc::abort();
        }
    }
}