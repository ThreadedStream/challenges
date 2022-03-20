libpreload.so: main.go
	go build -o libpreload.so -buildmode=c-shared && sudo cp libpreload.so /usr/lib

ffi: c/ffi.c
	gcc -o ffi c/ffi.c -lpreload
fixalloc: c/fixalloc.c
	gcc -o fixalloc c/fixalloc.c -lpreload
clean: 
	rm -rf main libpreload.*
