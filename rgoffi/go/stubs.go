package main

import (
	_ "unsafe"
)

// The following file contains all of the needed stubs and struct definitions

type fixalloc struct {
	size uintptr
	first func(arg, p *uint8)
	arg *uint8
	list *mlink
	chunk uintptr
	nchunk uintptr
	nalloc uintptr
	inuse uintptr
	stat *uint64
	zero bool
}

//go:notinheap
type mlink struct {
	next *mlink
}

type _type struct {
	size       uintptr
	ptrdata    uintptr
	hash       uint32
	tflag      int32
	align      uint8
	fieldAlign uint8
	kind       uint8
	equal      func(*uint8, *uint8) bool
	gcdata     *byte
	str        int32
	ptrToThis  int32
}

//go:linkname mallocgc runtime.mallocgc
func mallocgc(size uintptr, typ *_type) *uint8

//go:linkname internalAlloc runtime.sysAlloc
func internalAlloc(n uintptr, sysStat *uint64) *uint8

//go:linkname internalFree runtime.sysFree
func internalFree(v *uint8, n uintptr, sysStat *uint64)

//export allocate_go_memory
func allocate_go_memory(n uintptr) *uint8{
	return internalAlloc(n, nil)
}

//export deallocate_go_memory
func deallocate_go_memory(v *uint8, n uintptr) {
	internalFree(v, n, nil)
}

