extern crate libc;

#[link(name = "preload")]
extern "C" {
    fn allocate_go_memory(n: libc::uintptr_t) -> *mut u8;
    fn deallocate_go_memory(ptr: *mut u8, n: libc::uintptr_t);
}

fn main() {
    let ptr = unsafe { allocate_go_memory(32) };
    println!("{:p}", ptr);
    unsafe {
     *ptr = 0xca;
      println!("{}", *ptr);
      deallocate_go_memory(ptr, 32);
    }
}

