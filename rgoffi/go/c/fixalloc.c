#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../libpreload.h"

void panic(const char* message) {
	puts(message);
	abort();
}

static uint64_t _FixAllocChunk = 0x10 << 10;

struct mlink {
	struct mlink *next;
};


struct fixalloc {
	uintptr_t size;
	struct mlink *list;
	uintptr_t chunk;
	uint32_t nchunk;
	uint32_t nalloc;
	uintptr_t inuse;
	unsigned char zero;
};

typedef struct mlink mlink_t;
typedef struct fixalloc fixalloc_t;

void init(fixalloc_t *f, uintptr_t size) {
	if (size > _FixAllocChunk) {
		panic("fixalloc: fixalloc size too large");
	}

	uintptr_t min = sizeof(struct mlink);
	if (size < min) {
		size = min;
	}

	f->size = size;
	f->list = NULL;
	f->chunk = 0;
	f->nchunk = 0;
	f->nalloc = (uint32_t) (_FixAllocChunk / size * size);
	f->inuse = 0;
	f->zero = 1;
}

void *alloc(fixalloc_t *f) {
	if (f->size == 0) {
		panic("fixalloc: use of FixAlloc_Alloc before FixAlloc_Init");
	}
	if (f->list) {
		uint8_t *v = (uint8_t*) (f->list);
		f->list = f->list->next;
		f->inuse += f->size;
		if (f->zero) {
			memclr_noheap_pointers(v, f->size);
		}
		return (void*) v;
	}
	if (f->nchunk < f->size) {
		f->chunk = (uintptr_t) persistent_alloc(f->nalloc, 0, NULL);
		f->nchunk = f->nalloc;
	}
	f->chunk = f->chunk + f->size;
	f->nchunk -= f->size;
	f->inuse += f->size;
	return (void*) f->chunk;
}	

void dealloc(fixalloc_t *f, void *p) {
	f->inuse -= f->size;
	mlink_t *v = (mlink_t *) (p);
	v->next = f->list;
	f->list = v;
}

struct node {
	uint64_t data;
	struct node *next;
};

void test() {
	fixalloc_t *f = alloca(sizeof(fixalloc_t));
	assert(f);
	init(f, sizeof(struct node));
	struct node *n = alloc(f);
	assert(n);
	n->data = 0xcafebabe;
	n->next = NULL;
	printf("[&n = %p, data = %ld, next: %p]\n",n, n->data, n->next);
	puts("freeing no longer needed memory\n");
	dealloc(f, (void*)n);
	printf("[&n = %p, data = %ld, next: %p]\n", n, n->data, n->next);
}

int main() {
	test();
	return 0;
}

