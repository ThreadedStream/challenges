
#[link(name = "libkmalloc")]
extern "C" {
    pub fn kmalloc(nbytes: usize) -> *mut libc::c_void;
    pub fn krealloc(cp: *mut libc::c_void, nbytes: usize) -> *mut libc::c_void;
    pub fn kcalloc (nelem: usize, elsize: usize) -> *mut libc::c_void;
    pub fn kfree(cp: *mut libc::c_void);
}
