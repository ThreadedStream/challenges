#include <stdio.h>
#include <stdlib.h>

extern void throw(const char* message) {
	puts(message);
	abort();
}

