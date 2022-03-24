package main

import (
	"unsafe"
)

// slab-like allocator of fixed sizes

const (
	_FixAllocChunk = 0x10 << 10
)

type fixalloc struct {
	size uintptr // 8
	list *mlink // 8
	chunk uintptr // 8
	nchunk uint32 // 4
	nalloc uint32 // 4
	inuse uintptr // 8
	zero bool // 1
}

//go:notinheap
type mlink struct {
	next *mlink
}

func (f *fixalloc) init(size uintptr) {
	if size > _FixAllocChunk {
		panic("fixalloc: fixalloc size too large")
	}
	if min := unsafe.Sizeof(mlink{}); size < min {
		size = min
	}

	f.size = size
	f.list = nil
	f.chunk = 0 
	f.nchunk = 0
	f.nalloc = uint32(_FixAllocChunk / size * size)
	f.inuse = 0
	f.zero = true
}

func (f *fixalloc) alloc() unsafe.Pointer {
	if f.size == 0 {
		panic("fixalloc: use of FixAlloc_Alloc before FixAlloc_Init")
	}

	if f.list != nil {
		v := unsafe.Pointer(f.list)
		f.list = f.list.next
		f.inuse += f.size
		if f.zero {
			memclrNoHeapPointers((*uint8) (v), f.size)
		}
		return v
	}

	if uintptr(f.nchunk) < f.size {
		f.chunk = uintptr(unsafe.Pointer(persistentAlloc(uintptr(f.nalloc), 0, nil)))
		f.nchunk = f.nalloc
	}
	
	v := unsafe.Pointer(f.chunk)
	f.chunk = f.chunk + f.size
	f.nchunk -= uint32(f.size)
	f.inuse += f.size
	return v
}

func (f *fixalloc) free(p unsafe.Pointer) {
	f.inuse -= f.size
	v := (*mlink)(p)
	v.next = f.list
	f.list = v
}

