libpreload.so: main.go
	go build -o libpreload.so -buildmode=c-shared && sudo cp libpreload.so /usr/lib
ffi: c/ffi.c atomic_amd64.o
	gcc -o ffi c/ffi.c atomic_amd64.o -lpreload -lpthread
fixalloc: c/fixalloc.c
	gcc -o fixalloc c/fixalloc.c -lpreload
atomic_amd64.o: c/atomic_amd64.s
	gcc -m64 -c -o $@ -c $<
clean: 
	rm -rf main libpreload.*
