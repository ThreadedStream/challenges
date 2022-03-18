package main


import "unsafe"
import "C"

//export get_new_map
func get_new_map()  unsafe.Pointer {
    x := map[int]int{}
    return unsafe.Pointer(&x)
}

func main(){

}
