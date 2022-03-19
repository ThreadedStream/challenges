package main

// #include <stdlib.h>
import "C"

import "unsafe"

//export get_new_map
func get_new_map()  *C.char {
    x := new(int)
    *x = 32
    return (*C.char)(unsafe.Pointer(x))
}

//export c_malloc
func c_malloc() *C.char {
    ptr := (*C.char) (C.malloc(128))
    *ptr = 32
    return (*C.char)(ptr)
}

func main(){

}
