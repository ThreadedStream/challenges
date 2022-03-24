#ifndef LOCKRANK_OFF_H
#define LOCKRANK_OFF_H

#include "lockrank.h"

struct mutex;

typedef struct mutex mutex_t;

// lockRankStruct is embedded in mutex, but is empty when staticklockranking is
// disabled (the default)
struct lock_rank_struct{};

typedef struct lock_rank_struct lock_rank_struct_t;

void lock_init(mutex_t *l, lock_rank_t rank);

lock_rank_t get_lock_rank(mutex_t *l);

void lock_with_rank(mutex_t *l, lock_rank_t rank);

// This function may be called in nosplit context and thus must be nosplit.
//go:nosplit
void acquire_lock_rank(lock_rank_t rank);

void unlock_with_rank(mutex_t *l);

// This function may be called in nosplit context and thus must be nosplit.
//go:nosplit
void release_lock_rank(lock_rank_t rank);

void lock_with_rank_may_acquire(mutex_t *l , lock_rank_t rank);

//go:nosplit
void assert_lock_held(mutex_t *l);

//go:nosplit
void assert_rank_held(lock_rank_t r);

//go:nosplit
void world_stopped();

//go:nosplit
void world_started();

//go:nosplit
void assert_world_stopped();

//go:nosplit
void assert_world_stopped_or_lock_held(mutex_t *l);

#endif // LOCKRANK_OFF_H