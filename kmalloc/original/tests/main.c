#include <stdio.h>
#include <assert.h>

extern void *kmalloc(size_t nbytes);
extern void *krealloc(void *cp, size_t nbytes);
extern void *kcalloc (size_t nelem, size_t elsize);
extern void kfree(void *cp);

int main() {
    char *str = kcalloc(15, sizeof(char));
    assert(str != NULL);
    str = "hello mazafaka";
    kfree(str);
    str = NULL;
    return 0;
}