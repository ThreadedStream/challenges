#include <stdint.h>
#include "atomic_amd64.h"

uint32_t NOINLINE _load(uint32_t *ptr) {
	return *ptr;
}

void* NOINLINE _loadp(void **ptr) {
	return *ptr;
}

uint64_t NOINLINE _load64(uint64_t *ptr) {
	return *ptr;
}

