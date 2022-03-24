#ifndef STRING_H
#define STRING_H

// implementation of Go string

#define TMP_STRING_BUF_SIZE 32

typedef uint8_t[TMP_STRING_BUF_SIZE] tmp_buf_t;

struct string {
	uint8_t *ptr;
	uintptr_t len;
};

typedef struct string string_t;

string_t concatstrings();



#endif // STRING_H