#ifndef TYPE_H
#define TYPE_H

typedef uint8_t tflag_t;
typedef int32_t name_off_t;
typedef int32_t type_off_t;
typedef int32_t text_off_t;

enum {
	TFLAG_UNCOMMON = 1 << 0,
	TFLAG_EXTRA_STAR = 1 << 1,
	TFLAG_NAMED = 1 << 2,
	TFLAG_REGULAR_MEMORY = 1 << 3 // equal and hash can treat values of this type as a single region of t.size bytes
};

struct name {
	uint8_t *bytes;
};

typedef struct name name_t;

struct _type {
	uintptr_t size;
	uintptr_t ptrdata; // size of memory prefix holding all pointers
	uint32_t hash;
	tflag_t tflag;
	uint8_t align;
	uint8_t field_align;
	uint8_t kind;
	// function for comparing objects of this type
	// (ptr to object A, ptr to object B) -> ==?
	uint8_t (*equal)(void*, void*);
	// gcdata stores the GC type data for the garbage collector.
	// If the KindGCProg bit is set in kind, gcdata is a GC program.
	// Otherwise it is a ptrmask bitmap. See mbitmap.go for details.
	uint8_t *gcdata;
	name_off_t str;
	type_off_t ptr_to_this;
};

typedef struct _type _type_t; 

struct imethod {
	name_off_t name;
	type_off_t ityp;
};

typedef struct imethod imethod_t; 

struct interface_type {
	_type_t typ;
	name_t pkgpath;
	imethod_t *mhdr;
};

typedef struct interface_type interface_type_t;



#endif // TYPE_H
