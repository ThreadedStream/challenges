#include <stdio.h>
#include <assert.h>

extern void *kmalloc(size_t nbytes);
extern void *krealloc(void *cp, size_t nbytes);
extern void *kcalloc (size_t nelem, size_t elsize);
extern void kfree(void *cp);

union overhead {
    union overhead* next;
    struct {
        unsigned char ovu_magic;
        unsigned char ovu_index;
    }ovu;
};

static union overhead* buckets[30];

int main() {
    union overhead* ov = kmalloc(sizeof(union overhead));
    ov->next = kmalloc(sizeof(union overhead));
    buckets[0x2] = ov->next;
    ov->ovu.ovu_magic = 0xef;
    ov->ovu.ovu_index = 0x2;
    printf("buckets[0x2] = %p\n", buckets[0x2]);
    printf("ov->next = %p\n", ov->next);
    printf("ov->ovu.ovu_magic = %d\n", ov->ovu.ovu_magic);
    printf("ov->ovu.ovu_index = %d\n", ov->ovu.ovu_index);
    return 0;
}