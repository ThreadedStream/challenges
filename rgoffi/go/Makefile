SRC = $(wildcard gocruntime/*.c)
LINKER_FLAGS = -lpreload -lpthread
CLANG = clang
GCC = gcc

# %.o: $(SRC)
# 	gcc -c -o $@ $^ 

atomic_amd64.o: gocruntime/atomic_amd64.s
	gcc -c -o $@ $< -c 

# fixalloc.o: gocruntime/fixalloc.c
# 	gcc -c -o $@ $< 

# panic.o: gocruntime/panic.c
# 	gcc -c -o $@ $< -c 

# goruntime.o: gocruntime/goruntime.c
# 	gcc -c -o $@ $< 	

# atomic_amd64.o: gocruntime/internal/atomic/atomic_amd64.s
# 	gcc -m64 -c -o $@ -c $<

goruntime: libpreload.so $(SRC) atomic_amd64.o
	$(CLANG) -o $@  $^ $(LINKER_FLAGS)

libpreload.so: main.go
	go build -o libpreload.so -buildmode=c-shared && sudo cp libpreload.so /usr/lib

clean: 
	rm -rf main libpreload.* *.o
