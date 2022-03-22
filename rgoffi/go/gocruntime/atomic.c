#include <stdint.h>
#include <stdlib.h>
#include "atomic_amd64.h"
#include "atomic.h"

int32_t swap_int32(int32_t *addr, int32_t new) {
	return (int32_t) _xchg((uint32_t*) addr, (uint32_t) new);
}

int64_t swap_int64(int64_t *addr, int64_t new) {
	return (int64_t) _xchg64((uint64_t*) addr, (uint64_t) new);
}

uint32_t swap_uint32(uint32_t *addr, uint32_t new) {
	return _xchg(addr, new);
}

uint64_t swap_uint64(uint64_t * addr, uint64_t new) {
	return _xchg64(addr, new);
}

uintptr_t swap_uintptr(uintptr_t *addr, uintptr_t new) {
	return _xchg64(addr, new);
}

void* swap_pointer(void **addr , void *new) { 
	return _xchgp(addr, new);
}

uint8_t compare_and_swap_int32(int32_t *addr, int32_t old, int32_t new) {
	return _cas(addr, old, new);
}

uint8_t compare_and_swap_int64(int64_t* addr, int64_t old, int64_t new) {
	return _cas64(addr, old, new);
}

uint8_t compare_and_swap_uint32(uint32_t *addr, uint32_t old, uint32_t new) {
	return _cas((int32_t*) addr, (int32_t) old, (int32_t) new);
}

uint8_t compare_and_swap_uint64(uint64_t *addr, uint64_t old, uint64_t new) {
	return _cas64((int64_t*) addr, (int64_t) old, (int64_t) new);
}

uint8_t compare_and_swap_uintptr(uintptr_t *addr, uintptr_t old, uintptr_t new) {
	return _cas64((int64_t*) addr, (int64_t) old, (int64_t) new);
}

uint8_t compare_and_swap_pointer(void **addr , void *old, void *new) {
	return _casp1(addr, old, new);
}

int32_t add_int32(int32_t *addr, int32_t delta) {
	return _xadd((uint32_t *) addr, (uint32_t) delta);
}

uint32_t add_uint32(uint32_t *addr, uint32_t delta) {
	return _xadd(addr, delta);	
}

int64_t add_int64(int64_t *addr, int64_t delta) {
	return _xadd64((uint64_t*)addr, (uint64_t) delta);
}

uint64_t add_uint64(uint64_t *addr, uint64_t delta) {
	return _xadd64(addr, delta);
}

uintptr_t add_uintptr(uintptr_t* addr, uintptr_t delta) {
	return _xadd64(addr, delta);
}

int32_t load_int32(int32_t *addr) {
	return 0;
}

// LoadInt64 atomically loads *addr.
int64_t load_int64(int64_t *addr) { 
	return _load64((uint64_t*) addr);
}

// LoadUint32 atomically loads *addr. 
uint32_t load_uint32(uint32_t *addr) { 
	return _load(addr);
}

// LoadUint64 atomically loads *addr.
uint64_t load_uint64(uint64_t *addr) { 
	return _load64(addr);
}

// LoadUintptr atomically loads *addr.
uintptr_t load_uintptr(uintptr_t *addr) { 
	return _load64((uint64_t*) addr);
}

// LoadPointer atomically loads *addr.
void *load_pointer(void **addr) { 
	return _loadp(addr); 
}

// StoreInt32 atomically stores val into *addr.
void store_int32(int32_t *addr, int32_t val) {
	_store((uint32_t*) addr, (uint32_t) val);
}	

// StoreInt64 atomically stores val into *addr.
void store_int64(int64_t *addr, int64_t val) {
	_store64((uint64_t*) addr, (uint64_t) val);
}

// StoreUint32 atomically stores val into *addr.
void store_uint32(uint32_t *addr, uint32_t val) {
	_store(addr, val);
}

// StoreUint64 atomically stores val into *addr.
void store_uint64(uint64_t *addr, uint64_t val) {
	_store64(addr, val);
}

// StoreUintptr atomically stores val into *addr.
void store_uintptr(uintptr_t *addr, uintptr_t val) {
	_store64((uint64_t*) addr, (uint64_t) val);
}

// StorePointer atomically stores val into *addr.
void store_pointer(void **addr, void *val) {
	_storep(addr, val);
}	
