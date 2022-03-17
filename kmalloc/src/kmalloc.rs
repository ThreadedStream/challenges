use core::mem;
use core::ptr;
use libc::ptrace_syscall_info;
use libc_print::{libc_eprint};
use crate::kassert;

static PTR_SIZE: usize = mem::size_of::<usize>();
static MAGIC: u32 = 0xef;
static RMAGIC: u32 = 0x5555;
static PAGE_SIZE: u32 = 4096;
const NBUCKETS: usize = 30;
static REALLOC_SRCHLEN: i32 = 4;    /* 4 should be plenty, -1 =>'s whole list */

cfg_if::cfg_if! {
    if #[cfg(RCHECK)] {
        static RSLOP: u32 = mem::size_of<u16>();
    } else {
        static RSLOP: u32 = 0;
    }
}


#[repr(C)]
#[derive(Copy, Clone)]
struct Ovu {
    ovu_magic: u8,
    // magic number
    ovu_index: u8,
    // bucket
    #[cfg(RCHECK)]
    ovu_rmagic: u16,
    // range magic number
    #[cfg(RCHECK)]
    ovu_size: u32, // actual block size
}

#[repr(C)]
#[derive(Copy, Clone)]
union Overhead {
    ov_next: *mut Overhead,
    ovu: Ovu,
}

static mut NEXTF: [*mut Overhead; NBUCKETS] = [ptr::null_mut(); NBUCKETS];
static mut PAGE_SZ: u32 = 0;
static mut PAGE_BUCKET: i32 = -1;

#[cfg(MSTATS)]
static mut NMALLOC: [u32; NBUCKETS] = [0; NBUCKETS];

pub unsafe fn kmalloc(nbytes: usize) -> *mut u8 {
    let mut overhead: *mut Overhead = ptr::null_mut();
    let mut bucket: i32;
    let mut n: i32;
    let mut amt;

    if PAGE_SZ == 0 {
        PAGE_SZ = PAGE_SIZE;
        n = PAGE_SIZE as i32;
        overhead = libc::sbrk(0) as *mut Overhead;
        n = (n - mem::size_of::<Overhead>() as i32) - (overhead as i32 & (n - 1));
        if n < 0 {
            n += PAGE_SZ as i32;
        }
        if n > 0 {
            let interm = libc::sbrk(n as libc::intptr_t);
            if interm as isize == -1 {
                return ptr::null_mut();
            }
        }
        bucket = 0;
        amt = 8;
        while PAGE_SZ > amt {
            amt <<= 1;
            bucket += 1;
        }
        PAGE_BUCKET = bucket;
    }

    n = PAGE_SZ as i32 - mem::size_of::<Overhead>() as i32 - RSLOP as i32;
    if nbytes <= n as usize {
        cfg_if::cfg_if! {
            if #[cfg(RCHECK)] {
                amt = 16;
                bucket = 1;
            } else {
                amt = 8;
                bucket = 0;
            }
        }
        n = -(mem::size_of::<Overhead>() as i32 + RSLOP as i32);
    } else {
        amt = PAGE_SZ;
        bucket = PAGE_BUCKET;
    }
    while nbytes > (amt as i32 + n) as usize {
        amt <<= 1;
        if amt == 0 {
            return ptr::null_mut();
        }
        bucket += 1;
    }

    overhead = NEXTF[bucket as usize];

    if overhead == ptr::null_mut() {
        morecore(bucket);
        overhead = NEXTF[bucket as usize];
        if overhead == ptr::null_mut() {
            return ptr::null_mut();
        }
    }

    NEXTF[bucket as usize] = (*overhead).ov_next;
    (*overhead).ovu.ovu_magic = MAGIC as u8;
    (*overhead).ovu.ovu_index = bucket as u8;

    cfg_if::cfg_if! {
        if #[cfg(MSTATS)] {
            nmalloc[bucket] += 1;
        }
    }

    cfg_if::cfg_if! {
        if #[cfg(RCHECK)] {
            (*overhead).ov_size = (nbytes + RSLOP - 1) & ~(RSLOP - 1);
            (*overhead).ov_rmagic = RMAGIC;
  	        *(((op + 1) as usize + (*overhead).ov_size) as *mut u16) = RMAGIC;
        }
    }

    return (overhead as usize + mem::size_of::<Overhead>()) as *mut u8;
}

unsafe fn morecore(bucket: i32) {
    let mut overhead: *mut Overhead;
    let mut sz: i32;
    let mut amt: i32;
    let mut nblks: i32;


    sz = 1 << (bucket + 3);
    kassert!(sz > 0);

    if sz < PAGE_SZ as i32 {
        amt = PAGE_SZ as i32;
        nblks = amt / sz;
    } else {
        amt = sz + PAGE_SZ as i32;
        nblks = 1;
    }

    overhead = libc::sbrk(amt as libc::intptr_t) as *mut Overhead;
    // check failure
    if overhead as isize == -1 {
        return;
    }

    NEXTF[bucket as usize] = overhead;
    nblks -= 1;
    while nblks > 0 {
        (*overhead).ov_next = (overhead as i32 + sz) as *mut Overhead;
        overhead = (overhead as i32 + sz) as *mut Overhead;
        nblks -= 1;
    }
}

pub unsafe fn kcalloc(nelem: usize, elsize: usize) -> *mut u8 {
    let mut ptr = kmalloc(nelem * elsize);
    libc::memset(ptr as *mut libc::c_void, 0, nelem * elsize);
    return ptr;
}

pub unsafe fn realloc(cp: *mut u8, nbytes: usize) -> *mut u8 {
    let mut onb: u32;
    let mut i: i32;
    let mut overhead: *mut Overhead;
    let mut res: *mut u8;
    let mut was_alloced: i32 = 0;

    if cp == ptr::null_mut() {
        return kmalloc(nbytes);
    }
    if nbytes == 0 {
        kfree(cp);
        return ptr::null_mut();
    }
    overhead = (cp as usize - mem::size_of::<Overhead>()) as *mut Overhead;
    if (*overhead).ovu.ovu_magic == MAGIC as u8 {
        was_alloced += 1;
        i = (*overhead).ovu.ovu_index as i32;
    } else {
        // Already free, doing "compaction".
        //    Search for the old block of memory on the
        //    free list.  First, check the most common
        //     case (last element free'd), then (this failing)
        //     the last ``realloc_srchlen'' items free'd.
        // If all lookups fail, then assume the size of
        //     the memory block being realloc'd is the
        //     largest possible (so that all "nbytes" of new
        //     memory are copied into).  Note that this could cause
        //     a memory fault if the old area was tiny, and the moon
        //     is gibbous.  However, that is very unlikely.
        //

        i = findbucket(overhead, 1);
        let temp = findbucket(overhead, REALLOC_SRCHLEN);
        if i < 0 && temp < 0 {
            i = NBUCKETS as i32;
        }
    }

    onb = 1 << (i + 3);
    if onb < PAGE_SZ {
        onb -= mem::size_of::<Overhead>() as u32 + RSLOP as u32;
    } else {
        onb += PAGE_SZ - mem::size_of::<Overhead>() as u32 - RSLOP as u32;
    }

    if was_alloced > 0 {
        if i > 0 {
            i = 1 << (i + 2);
            if i < PAGE_SZ as i32 {
                i -= mem::size_of::<Overhead>() as i32 + RSLOP as i32;
            } else {
                i += PAGE_SZ as i32 - mem::size_of::<Overhead>() as i32 - RSLOP as i32;
            }
        }
        if nbytes <= onb as usize && nbytes > i as usize {
            cfg_if::cfg_if! {
                    if #[cfg(RCHECK)] {
                        (*(*overhead).ovu).ovu_size =  (nbytes + RSLOP - 1) & ~(RSLOP - 1);
                        (op + 1) as usize + (*(*overhead).ovu).ovu_size = RMAGIC;
                    }
            }
            return cp;
        } else {
            kfree(cp);
        }
    }
    res = kmalloc(nbytes);
    if res == ptr::null_mut() {
        return ptr::null_mut();
    }

    if cp != res {
        let p = if nbytes < onb as usize {
            nbytes
        } else {
            onb as usize
        };

        libc::memcpy(res as *mut libc::c_void, cp as *const libc::c_void, p);
    }

    return res;
}

unsafe fn findbucket(freep: *mut Overhead, srchlen: i32) -> i32 {
    let mut p: *mut Overhead;
    let mut i: i32;
    let mut j: i32;

    for i in 0..NBUCKETS as i32 {
        j = 0;
        p = NEXTF[i as usize];
        loop {
            let inter = if p as usize == 0 || j == 0 {
                0
            } else {
                1
            };

            if inter as usize == srchlen as usize {
                break;
            }
            if p == freep {
                return i;
            }
            j += 1;
        }
    }

    return -1;
}

pub unsafe fn kfree(cp: *mut u8) {
    let mut size: i32;
    let mut overhead: *mut Overhead;

    if cp == ptr::null_mut() {
        return;
    }
    overhead = (cp as usize - mem::size_of::<Overhead>()) as *mut Overhead;
    kassert!((*overhead).ovu.ovu_magic as u32 == MAGIC);
    //kassert!((*(*overhead).ovu).ovu_rmagic == RMAGIC);
    //ASSERT(*(u_short *)(op + 1) + op->ov_size) == RMAGIC);
    size = (*overhead).ovu.ovu_index as i32;
    kassert!(size < NBUCKETS as i32);
    (*overhead).ov_next = NEXTF[size as usize];
    NEXTF[size as usize] = overhead;
}

unsafe fn kmalloc_usable_size(cp: *mut u8) -> usize {
    let mut overhead: *mut Overhead;

    if cp == ptr::null_mut() {
        return 0;
    }

    overhead = (cp as usize - mem::size_of::<Overhead>()) as *mut Overhead;
    kassert!((*overhead).ovu.ovu_magic as u32 == MAGIC);
    return (*overhead).ovu.ovu_index as usize;
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn alloc() {
        unsafe {
            let p = kmalloc(15);
            *p = 32;
            assert_eq!(*p, 32);
            kfree(p);
        }
    }

    fn stress_test() {

    }
}