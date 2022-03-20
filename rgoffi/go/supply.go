package main


import (
	"unsafe"
	"fmt"
	"math/rand"
)

func printContents(ptr *uint8, size uintptr) {
	unsafePtr := unsafe.Pointer(ptr)

	var i uintptr
	for i = 0; i < size; i++ {
		fmt.Printf("%x\n", *((*uint8)(unsafe.Add(unsafePtr, i))))
	} 
}

func writeContents(ptr *uint8, size uintptr) {
	unsafePtr := unsafe.Pointer(ptr)
	var i uintptr
	for i = 0; i < size; i++ {
		*((*uint8)(unsafe.Add(unsafePtr, i))) = uint8(rand.Int())
	} 	
}