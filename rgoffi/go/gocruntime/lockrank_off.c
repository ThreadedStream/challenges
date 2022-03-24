#include <stdint.h>
#include "lockrank_off.h"

void lock_init(mutex_t *l, lock_rank_t rank) {}

lock_rank_t get_lock_rank(mutex_t *l) {
	return 0;
}

void lock_with_rank(mutex_t *l, lock_rank_t rank) {
	runtime_lock2((uint8_t*) l);
}

// This function may be called in nosplit context and thus must be nosplit.
//go:nosplit
void acquire_lock_rank(lock_rank_t rank) {
}

void unlock_with_rank(mutex_t *l) {
	runtime_unlock2((uint8_t*) l);
}

// This function may be called in nosplit context and thus must be nosplit.
//go:nosplit
void release_lock_rank(lock_rank_t rank) {
}

void lock_with_rank_may_acquire(mutex_t *l , lock_rank_t rank) {
}

//go:nosplit
void assert_lock_held(mutex_t *l) {
}

//go:nosplit
void assert_rank_held(lock_rank_t r) {
}

//go:nosplit
void world_stopped() {
}

//go:nosplit
void world_started() {
}

//go:nosplit
void assert_world_stopped() {
}

//go:nosplit
void assert_world_stopped_or_lock_held(mutex_t *l) {
}
