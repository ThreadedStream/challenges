#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include "atomic.h"
#include "../libpreload.h"

typedef void * (*MALLOCGC_FUNC_PTR) (int64_t size, void *typ, uint8_t needzero);

void write_memory(uint8_t *ptr, intptr_t size) {
	srand(time(NULL));
	for (intptr_t i = 0; i < size; i++) {
		*(ptr + i) = rand() % 0xff;
	}
}

void read_memory(uint8_t* ptr, intptr_t size) {
	srand(time(NULL));
	for (intptr_t i = 0; i < size; i++) {
		printf("0x%x\n", *(ptr + i));
	}	
}

void *modify_ptr_1(void *ptr) {
    int32_t *p = (int32_t *) ptr;
    for (int32_t i = 0; i < 15; i++) {
        printf("[thread1] i = %d\n", i);
        _xadd((uint32_t volatile*) p, (uint32_t) i);
    }
    return NULL;
}

void *modify_ptr_2(void *ptr) {
    int32_t *p = (int32_t *) ptr;
    for (int32_t i = 0; i < 15; i++) {
        printf("[thread2] i = %d\n", i);
        _xadd((uint32_t volatile*) p, (uint32_t) -i);
    }
    return NULL;
}

void test_atomicity() {
    int32_t *ptr = (int32_t*) allocate_go_memory(16);
    pthread_t t1, t2;
    int t1_res = pthread_create(&t1, NULL, modify_ptr_1, (void*)ptr);
    assert(t1_res == 0);
    int t2_res = pthread_create(&t2, NULL, modify_ptr_2, (void*)ptr);
    assert(t2_res == 0);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("*ptr = %d\n", *ptr);
    deallocate_go_memory((uint8_t*) ptr, 16);
}

int main() {
    uint32_t *ptr = (uint32_t*) allocate_go_memory(sizeof(uint32_t));
    *ptr = 80;
    uint32_t val = 0xca;
    uint32_t *temp = (uint32_t*) swap_pointer((void**) &ptr, (void*) &val);
    printf("*ptr = %d\n val = %d\n", *ptr, val);
}

