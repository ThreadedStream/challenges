#![no_std]

extern crate alloc;

use alloc::alloc::handle_alloc_error;
use core::alloc::{GlobalAlloc, Layout};

mod kassert;
mod kmalloc;
mod kmalloc_c;

pub struct Kmalloc;

unsafe impl GlobalAlloc for Kmalloc {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        let ptr = kmalloc::kmalloc(layout.size());
        if ptr.is_null() {
            handle_alloc_error(layout);
        }
        ptr as *mut u8
    }

    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) {
        kmalloc::kfree(ptr);
    }
}

