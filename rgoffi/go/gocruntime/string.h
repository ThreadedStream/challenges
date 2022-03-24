#ifndef STRING_H
#define STRING_H

// implementation of Go string

#define TMP_STRING_BUF_SIZE 32

typedef struct string string_t;
typedef struct string_list string_list_t;

struct string {
	uint8_t *str;
	uintptr_t len;
};

struct string_list {
	string_t *s;
	string_list_t *tail;
};


string_list_t *new_string_list();
string_t *concatstrings(uint8_t *buf[TMP_STRING_BUF_SIZE], string_list_t *a);
uint8_t string_data_on_stack(string_list_t *slice, uintptr_t idx);
string_list_t *string_at(string_list_t *slice, uintptr_t idx);
string_t *slice_byte_to_string(uint8_t *buf[TMP_STRING_BUF_SIZE], uint8_t *ptr, int n);
string_t *empty_string();

#endif // STRING_H