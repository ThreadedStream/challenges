
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
