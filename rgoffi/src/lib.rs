extern crate libc;
// calling rust code from go and vice versa
// objectives:
//  1) try calling a go function inside rust environment
//  2) do the same thing, but now with roles swapped
//  3) write some interface for automating process of calling external go functions
use num::complex::Complex;

type GoInt8 = i8;
type GoUint8 = u8;
type GoInt16 = i16;
type GoUint16 = u16;
type GoInt32 = i32;
type GoUint32 = u32;
type GoInt64 = i64;
type GoUint64 = u64;
type GoUintptr = usize;
type GoInt = GoInt64;
type GoUint = GoUint64;
type GoFloat32 = f32;
type GoFloat64 = f64;
type GoComplex64 = Complex::<f32>;
type GoComplex128 = Complex::<f64>;
type GoMap = *mut libc::c_void;
type GoChan = *mut libc::c_void;

struct GoString {
    p: *mut libc::c_char,
    n: libc::ptrdiff_t
}

struct GoInterface {
    t: *mut libc::c_void,
    v: *mut libc::c_void
}

struct GoSlice {
    data: *mut libc::c_void,
    len: GoInt,
    cap: GoInt
}

#[link(name = "preload")]
extern "C" {
    fn get_new_map() -> *mut GoMap;
}

fn main() {
    let go_map = unsafe { get_new_map() };

    println!("{:p}", go_map);
}

