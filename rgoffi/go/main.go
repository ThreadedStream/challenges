package main

import "C"

import (
	"fmt"
)


func main() {
	fmt.Printf("attempt to allocate memory through stub...\n")
	ptr := (*uint8) (internalAlloc(256, nil))
	fmt.Printf("%p\n", ptr)
	writeContents(ptr, 16)
	printContents(ptr, 16)
	// fmt.Printf("*ptr + 1 = %d\n", int(*(ptr + uint8(1))))
}
