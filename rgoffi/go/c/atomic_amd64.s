.section .text

.global _cas
.global _casp1
.global _cas64
.global _casint32
.global _casint64
.global _casuintptr
.global _casrel
.global _xadd

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

