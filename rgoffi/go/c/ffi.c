#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "../libpreload.h"

typedef void * (*MALLOCGC_FUNC_PTR) (int64_t size, void *typ, uint8_t needzero);

void write_memory(uint8_t *ptr, intptr_t size) {
	srand(time(NULL));
	for (intptr_t i = 0; i < size; i++) {
		*(ptr + i) = rand() % 0xff;
	}
}

void read_memory(uint8_t* ptr, intptr_t size) {
	srand(time(NULL));
	for (intptr_t i = 0; i < size; i++) {
		printf("0x%x\n", *(ptr + i));
	}	
}

int main() {
	// puts("allocating a fresh memory...\n");
	// uint8_t *ptr = allocate_go_memory(256);
	// write_memory(ptr, 16);
	// read_memory(ptr, 16);
	// printf("ptr = %p\n", ptr);
	// printf("deallocating it...\n");
	// deallocate_go_memory(ptr, 256);

	uint8_t *ptr = persistent_alloc(256, 16, NULL);
	printf("ptr = %p\n", ptr);
	deallocate_go_memory(ptr, 256);
	return 0;
}
