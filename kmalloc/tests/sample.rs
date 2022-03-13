extern crate kmalloc;

use kmalloc::Kmalloc;

#[global_allocator]
static GLOBAL: Kmalloc = Kmalloc;

#[test]
fn sanity() {
    let mut  v = Vec::new();
    v.push(2);
    drop(v);
}
