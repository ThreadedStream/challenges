use core::mem;
use core::ptr;
use libc::ptrace_syscall_info;
use libc_print::{libc_eprint};
use crate::kassert;

static MAGIC: u32 = 0xef;
static RMAGIC: u32 = 0x5555;
static PAGE_SIZE: u32 = 4096;
const NBUCKETS: usize = 30;

cfg_if::cfg_if! {
    if #[cfg(RCHECK)] {
        static RSLOP: u32 = mem::size_of<u16>();
    } else {
        static RSLOP: u32 = 0;
    }
}

#[repr(C)]
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
union Overhead {
    ov_next: *mut Overhead,
    ovu: *mut Ovu,
}

static mut NEXTF: [*mut Overhead; NBUCKETS] = [ptr::null_mut(); NBUCKETS];
static mut PAGE_SZ: u32 = u32::MAX;
static mut PAGE_BUCKET: u32 = u32::MAX;

#[cfg(MSTATS)]
static mut NMALLOC: [u32; NBUCKETS] = [0; NBUCKETS];

unsafe fn kmalloc(nbytes: usize) -> *mut u8 {
    let mut overhead: *mut Overhead = ptr::null_mut();
    let mut bucket;
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
            if libc::sbrk(n as libc::intptr_t) as usize == usize::MAX {
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
                amt = 8;
                bucket = 0;
            } else {
                amt = 16;
                bucket = 1;
            }
        }
        n = -(mem::size_of::<Overhead>() as i32 + RSLOP as i32);
    } else {
        amt = PAGE_SZ;
        bucket = PAGE_BUCKET;
    }
    while nbytes > (amt + n as u32) as usize {
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
    (*(*overhead).ovu).ovu_magic = MAGIC as u8;
    (*(*overhead).ovu).ovu_index = bucket as u8;

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

    return (overhead as usize + 1) as *mut u8;
}

unsafe fn morecore(bucket: u32) {
    let mut overhead: *mut Overhead;
    let mut sz;
    let mut amt;
    let mut nblks;


    sz = 1 << (bucket + 3);
    kassert!(sz > 0);

    if sz <= 0 {
        return;
    }

    if sz < PAGE_SZ {
        amt = PAGE_SZ;
        nblks = amt / sz;
    } else {
        amt = sz + PAGE_SZ;
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
        (*overhead).ov_next = (overhead as usize + sz as usize) as *mut Overhead;
        overhead = (overhead as usize + sz as usize) as *mut Overhead;
    }
}

unsafe fn kcalloc(nelem: usize, elsize: usize) -> *mut u8 {
    let mut ptr = kmalloc(nelem * elsize);
    libc::memset(ptr as *mut libc::c_void, 0, nelem * elsize);
    return ptr;
}

unsafe fn realloc(cp: *mut u8, nbytes: usize) -> *mut u8 {
    let mut onb: u32;
    let i: i32;
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
    overhead = cp - mem::size_of::<Overhead>() as *mut Overhead;
    if (*(*overhead).ovu).ovu_magic == MAGIC as u8{
        was_alloced += 1;
        i = (*(*overhead).ovu).ovu_index as i32;
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
        let temp = findbucket(overhead, realloc_srchlen);
        if i < 0 && temp < 0 {
            i = NBUCKETS as i32;
        }

        onb = 1 << (i + 3);
        if onb < PAGE_SZ {
            onb -= mem::size_of::<Overhead>() + RSPLOP;
        } else {
            onb += PAGE_SZ
        }
    }
}

unsafe fn findbucket(freep: *mut Overhead, srchlen: i32) -> i32 {

}