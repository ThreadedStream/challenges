.section .text

.global _cas
.global _casp1
.global _cas64
.global _casint32
.global _casint64
.global _casuintptr
.global _casrel
.global _xadd
.global _xadd64
.global _xchg
.global _xchg64
.global _xchgp
.global _store
.global _storep
.global _store64
.global _store8
.global _or8
.global _and8
.global _or
.global _and

# uint8_t _cas(int32_t *ptr, int32_t old, int32_t new)
_cas:
	movl %esi, %eax
	lock
	cmpxchg %edx, (%rdi)
	sete %al
	ret

# uint8_t _casp1(void **val, void* old, void *new)
_casp1:
	mov %rsi, %rax
	lock
	cmpxchg %rdx, (%rsi)
	sete %al
	ret

# uint8_t _cas64(uint64_t* val, uint64_t old, uint64_t new)
_cas64: 
	movq %rsi, %rax
	lock
	cmpxchg %rdx, (%rdi)
	sete %al
	ret

_casint32:
	jmp _cas

_casint64:
	jmp _cas64

_casuintptr:
	jmp _cas64

_casrel:
	jmp _cas

# uint32_t _xadd(uint32_t volatile *val, int32_t delta)
_xadd:
	movl %esi, %eax
	lock
	xaddl %eax, (%rdi)
	addl %esi, %eax
	ret

# uint64_t _xadd64(uint64_t volatile *val, int64_t delta)
_xadd64:
	movq %rsi, %rax
	lock
	xaddq %rax, (%rdi)
	addq %rsi, %rax
	ret

# uint32_t _xchg(uint32_t *ptr, uint32_t new)
_xchg:
	movl %esi, %eax
	xchgl %eax, (%rdi)
	ret

# uint64_t _xchg64(uint64_t *ptr, uint64_t new)
_xchg64:
	movq %rsi, %rax
	xchg %rax, (%rdi)
	ret

_xchgp:
	jmp _xchg64

_store_no_wb:
	xchgq %rsi, (%rdi)
	ret

_store:
	xchgl %esi, (%rdi)
	ret

_store8:
	xchgb %sil, (%rdi)
	ret

_store64:
	xchgq %rsi, (%rdi)
	ret

_storep:
	jmp _store64

_store_int32:
	jmp _store

_store_int64:
	jmp _store64

_store_uintptr:
	jmp _store64

_store_rel:
	jmp _store

_store_rel_64:
	jmp _store64

_store_rel_uintptr:
	jmp _store64

# uint8_t _or8(uint8_t volatile *ptr, uint8_t val)
_or8:
	lock 
	orb %sil, (%rdi)
	ret

# uint8_t _and8(uint8_t volatile *ptr, uint8_t val)
_and8:
	lock 
	andb %sil, (%rdi)
	ret

# uint32_t _or(uint32_t volatile *ptr, uint32_t val)
_or:
	lock	
	orl %esi, (%rdi)
	ret

# uint32_t _and(uint32_t volatile *ptr, uint32_t val)
_and:
	lock
	andl %esi, (%rdi)
	ret

