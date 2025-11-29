/*
 * MiniVec - A minimalist version of Vec
 */

#ifndef CCOLL_MINIVEC_H
#define CCOLL_MINIVEC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../ccoll-codes.h"

#define CCOLL_MINIVEC_MIN_CAPACITY 8
#define CCOLL_MINIVEC_MIN_ALIGNMENT 4
#define CCOLL_MINIVEC_DEFAULT_ALIGNMENT 8
#define CCOLL_MINIVEC_GROWTH_STRATEGY CCOLL_GROWTH_STRATEGY_DOUBLE
#define CCOLL_MINIVEC_ARG_CHECK 1
#define CCOLL_MINIVEC_DEBUG 1

typedef struct MiniVec {
	size_t size;
	size_t capacity;
	size_t item_size;
	void *data;
} MiniVec;

struct _MiniVec_init_opts {
	size_t capacity;
	size_t alignment;
};

struct _MiniVec_change_capacity_opts {
	size_t alignment;
};

struct _MiniVec_growth_capacity_opts {
	size_t alignment;
	CCOLL_GROWTH_STRATEGY growth_strategy;
};

#include "../src/minivec/checks.h"
#include "../src/minivec/debug.h"
#include "../src/minivec/helpers.h"

// Internal implementation of MiniVec_init function
MiniVec *
_MiniVec_init(size_t sizeof_item, const struct _MiniVec_init_opts *opts);
// Initializes MiniVec data structure
// - size of single item
// - options like
// - - capacity - initial capacity
// - - alignment - alignment (need to be specified also in later calls)
#define MiniVec_init(sizeof_item, ...)                                         \
	_MiniVec_init(                                                           \
	    sizeof_item, &(const struct _MiniVec_init_opts){__VA_ARGS__}         \
	)

// TODO: make docs
int _MiniVec_free(MiniVec *vec);
// Free the MiniVec data structure
// - pointer to MiniVec data structure
#define MiniVec_free_safe(vec)                                                 \
	({                                                                       \
		int _result = CCOLL_NULL;                                          \
		if (vec) {                                                         \
			_result = _MiniVec_free(vec);                                \
			if (_result == CCOLL_SUCCESS) {                              \
				(vec) = NULL;                                          \
			}                                                            \
		}                                                                  \
		_result;                                                           \
	})

// TODO: make docs
int _MiniVec_change_capacity(
    MiniVec *vec,
    const size_t capacity,
    const struct _MiniVec_change_capacity_opts *opts
);
// TODO: make docs
#define MiniVec_change_capacity(vec, capacity, ...)                            \
	_MiniVec_change_capacity(                                                \
	    vec, capacity,                                                       \
	    &(const struct _MiniVec_change_capacity_opts){__VA_ARGS__}           \
	)

// TODO: make docs
int _MiniVec_alloc(
    MiniVec *vec,
    const size_t idxs,
    const struct _MiniVec_growth_capacity_opts *opts
);
// TODO: make docs
#define MiniVec_alloc(vec, idxs, ...)                                          \
	_MiniVec_alloc(                                                          \
	    vec, idxs,                                                           \
	    &(const struct _MiniVec_growth_capacity_opts){__VA_ARGS__}           \
	)

// TODO: make docs
int _MiniVec_reserve_additional(
    MiniVec *vec,
    const size_t idxs,
    const struct _MiniVec_growth_capacity_opts *opts
);
// TODO: make docs
#define MiniVec_reserve_additional(vec, idxs, ...)                             \
	_MiniVec_reserve_additional(                                             \
	    vec, idxs,                                                           \
	    &(const struct _MiniVec_growth_capacity_opts){__VA_ARGS__}           \
	)

// TODO: make docs
int _MiniVec_shrink(
    MiniVec *vec, const struct _MiniVec_change_capacity_opts *opts
);
// TODO: make docs
#define MiniVec_shrink()

int MiniVec_set(MiniVec *vec, const void *data, size_t idx);
int MiniVec_set_range(
    MiniVec *vec,
    const void *data,
    const size_t start_idx,
    const size_t quantity
);

static inline void *
MiniVec_get_unchecked_ptr(const MiniVec *vec, const size_t idx) {
	return (char *)vec->data + (idx * vec->item_size);
}
static inline void *MiniVec_get_ptr(const MiniVec *vec, const size_t idx) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (idx >= vec->size) return NULL;

	return MiniVec_get_unchecked_ptr(vec, idx);
}

MiniVec *MiniVec_get_unchecked_clone(const MiniVec *vec, const size_t idx);
MiniVec *MiniVec_get_clone(const MiniVec *vec, const size_t idx);

int MiniVec_push_back(MiniVec *vec, const void *data);
int MiniVec_push_front(MiniVec *vec, const void *data);
int MiniVec_push_back_range(MiniVec *vec, const void *data, size_t quantity);
int MiniVec_push_front_range(MiniVec *vec, const void *data, size_t quantity);

int MiniVec_pop_back(MiniVec *vec);
int MiniVec_pop_front(MiniVec *vec);

int MiniVec_insert(MiniVec *vec, const void *data, const size_t idx);
int MiniVec_insert_range(
    MiniVec *vec,
    const void *data,
    const size_t start_idx,
    const size_t quantity
);

int MiniVec_remove(MiniVec *vec, const size_t idx);
int MiniVec_remove_range(
    MiniVec *vec, const size_t from_idx, const size_t to_idx
);
int MiniVec_remove_all(MiniVec *vec);

int MiniVec_append(MiniVec *base, const MiniVec *vec);
int MiniVec_append_clone(MiniVec *vec1, MiniVec *vec2, MiniVec **new_vec);
int MiniVec_split(MiniVec *base, MiniVec **new_vec, const size_t idx);
int MiniVec_split_clone(
    const MiniVec *base,
    MiniVec **new_vec1,
    MiniVec **new_vec2,
    const size_t idx
);

int MiniVec_slice(MiniVec *vec, const size_t from_idx, const size_t to_idx);
MiniVec *MiniVec_slice_clone(
    const MiniVec *vec, const size_t from_idx, const size_t to_idx
);

static inline bool MiniVec_is_empty(MiniVec *vec) {
	if (vec->size == 0)
		return true;
	else
		return false;
}

int MiniVec_swap(MiniVec *vec, const size_t idx1, const size_t idx2);
int MiniVec_fill(MiniVec *vec, void *data);

#endif
