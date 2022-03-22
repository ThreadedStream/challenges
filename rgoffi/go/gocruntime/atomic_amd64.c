#include <stdint.h>
#include "atomic_amd64.h"

uint32_t __attribute__((noinline)) _load(uint32_t *ptr) {
	return *ptr;
}

void* __attribute__((noinline)) _loadp(void **ptr) {
	return *ptr;
}

uint64_t __attribute__((noinline)) _load64(uint64_t *ptr) {
	return *ptr;
}

