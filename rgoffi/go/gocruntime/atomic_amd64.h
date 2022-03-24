#ifndef ATOMIC_AMD64_H
#define ATOMIC_AMD64_H

#ifdef __GNUC__
	#define NOINLINE __attribute__((noinline))
#else
	#define NOINLINE
#endif // __GNUC__


uint32_t NOINLINE _load(uint32_t *ptr);

void* NOINLINE _loadp(void **ptr);

uint64_t NOINLINE _load64(uint64_t *ptr);

// compare and set
extern uint8_t _cas(int32_t *ptr, int32_t old, int32_t new);

// 64-bit compare and set
extern uint8_t _cas64(int64_t *ptr, int64_t old, int64_t new);

// compare and set (pointers)
extern uint8_t _casp1(void **ptr, void* old, void* new);

// eXchange add
extern uint32_t _xadd(uint32_t volatile *val, int32_t delta);

// 64-bit eXchange add 
extern uint64_t _xadd64(uint64_t volatile* val, int64_t delta);

// 32-bit eXchange 
extern uint32_t _xchg(uint32_t *ptr, uint32_t new);

// 64-bit eXchange
extern uint64_t _xchg64(uint64_t *ptr, uint64_t new);

// pointer exchange
extern void * _xchgp(void **ptr, void *new);

// 32-bit store
extern void _store(uint32_t *ptr, uint32_t val);

// 64-bit store
extern void _store64(uint64_t *ptr, uint64_t val);

// pointer store
extern void _storep(void **ptr, void *val);

// 8-bit store
extern void _store8(uint8_t *ptr, uint8_t val);

// 8-bit or
extern uint8_t _or8(uint8_t volatile *ptr, uint8_t val);

// 8-bit and
extern uint8_t _and8(uint8_t volatile *ptr, uint8_t val);

// 32-bit or
extern uint32_t _or(uint32_t volatile *ptr, uint32_t val);

// 32-bit and
extern uint32_t _and(uint32_t volatile *ptr, uint32_t val);


#endif // ATOMIC_AMD64_H
