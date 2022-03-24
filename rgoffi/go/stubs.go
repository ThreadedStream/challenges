package main

import (
	_ "unsafe"
)

// The following file contains all of the needed stubs and struct definitions


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

// //go:linkname mallocgc runtime.mallocgc
// func mallocgc(size uintptr, typ *_type) *uint8

//go:linkname systemAlloc runtime.sysAlloc
func systemAlloc(n uintptr, sysStat *uint64) *uint8

//go:linkname systemFree runtime.sysFree
func systemFree(v *uint8, n uintptr, sysStat *uint64)

//go:linkname persistentAlloc runtime.persistentalloc
func persistentAlloc(size, align uintptr, sysStat *uint64) *uint8

//go:linkname memclrNoHeapPointers runtime.memclrNoHeapPointers
func memclrNoHeapPointers(ptr *uint8, size uintptr)

//go:linkname lock2 runtime.lock2
func lock2(m *uint8)

//go:linkname unlock2 runtime.unlock2
func unlock2(m *uint8)

//go:linkname extendRandom runtime.extendRandom
func extendRandom(r []byte, n int)

// //go:linkname main.(*fixalloc).alloc runtime.(*fixalloc).alloc
// func (f *fixalloc) alloc() *uint8
