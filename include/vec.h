/* Vec - A minimalist version of Vec */

#ifndef CCOLL_VEC_H
#define CCOLL_VEC_H

#define CCOLL_DEBUG 1
#include "../util/debug.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../util/ccoll-codes.h"

#define CCOLL_VEC_MIN_CAPACITY 8
#define CCOLL_VEC_MIN_ALIGNMENT 4
#define CCOLL_VEC_DEFAULT_ALIGNMENT 8
#define CCOLL_VEC_GROWTH_STRATEGY CCOLL_GROWTH_STRATEGY_DOUBLE
#define CCOLL_VEC_ARG_CHECK 1

typedef struct Vec {
	size_t size;
	size_t capacity;
	size_t item_size;
	void *data;
} Vec;

struct _Vec_init_opts {
	size_t capacity;
	size_t alignment;
};

// Internal implementation of Vec_init function
Vec *_Vec_init(size_t sizeof_item, const struct _Vec_init_opts *opts);
// Initializes Vec data structure
// - size of single item
// - options like
// - - capacity - initial capacity
// - - alignment - alignment (need to be specified also in later calls)
#define Vec_init(sizeof_item, ...) _Vec_init(						 \
	sizeof_item, &(const struct _Vec_init_opts){__VA_ARGS__}			 \
)

// Internal implementation of Vec_free function
int _Vec_free(Vec *vec);
// Free the Vec data structure
// - pointer to Vec data structure
#define Vec_free(vec)										 \
	({                                                                       \
		int _result = _Vec_free(vec);							 \
		if (_result == CCOLL_SUCCESS) {						 \
			(vec) = NULL;								 \
		} else {										 \
			CCOLL_ERROR("free returned error");					 \
		}											 \
		_result;                                                           \
	})

struct _Vec_change_capacity_opts {
	size_t alignment;
};
// TODO: make docs
int _Vec_change_capacity(
    Vec *vec,
    const size_t capacity,
    const struct _Vec_change_capacity_opts *opts
);
// TODO: make docs
#define Vec_change_capacity(vec, capacity, ...)						 \
	_Vec_change_capacity(									 \
	    vec, capacity,                                                       \
	    &(const struct _Vec_change_capacity_opts){__VA_ARGS__}			 \
	)

struct _Vec_growth_capacity_opts {
	size_t alignment;
	CCOLL_GROWTH_STRATEGY growth_strategy;
};
// TODO: make docs
int _Vec_alloc(
    Vec *vec,
    const size_t idxs,
    const struct _Vec_growth_capacity_opts *opts
);
// TODO: make docs
#define Vec_alloc(vec, idxs, ...)                                          \
	_Vec_alloc(                                                          \
	    vec, idxs,                                                           \
	    &(const struct _Vec_growth_capacity_opts){__VA_ARGS__}           \
	)

// TODO: make docs
int _Vec_reserve_additional(
    Vec *vec,
    const size_t idxs,
    const struct _Vec_growth_capacity_opts *opts
);
// TODO: make docs
#define Vec_reserve_additional(vec, idxs, ...)                             \
	_Vec_reserve_additional(                                             \
	    vec, idxs,                                                           \
	    &(const struct _Vec_growth_capacity_opts){__VA_ARGS__}           \
	)

// TODO: make docs
int _Vec_shrink(
    Vec *vec, const struct _Vec_change_capacity_opts *opts
);
// TODO: make docs
#define Vec_shrink()

int Vec_set(Vec *vec, const size_t idx, const void *data);

int Vec_set_range(
    Vec *vec,
    const void *data,
    const size_t start_idx,
    const size_t quantity
);

static inline void *
Vec_get_unchecked_ptr(const Vec *vec, const size_t idx) {
	return (char *)vec->data + (idx * vec->item_size);
}
static inline void *Vec_get_ptr(const Vec *vec, const size_t idx) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (idx >= vec->size) return NULL;

	return Vec_get_unchecked_ptr(vec, idx);
}

Vec *Vec_get_unchecked_clone(const Vec *vec, const size_t idx);
Vec *Vec_get_clone(const Vec *vec, const size_t idx);

int Vec_push_back(Vec *vec, const void *data);
int Vec_push_front(Vec *vec, const void *data);
int Vec_push_back_range(Vec *vec, const void *data, size_t quantity);
int Vec_push_front_range(Vec *vec, const void *data, size_t quantity);

int Vec_pop_back(Vec *vec);
int Vec_pop_front(Vec *vec);

int Vec_insert(Vec *vec, const void *data, const size_t idx);
int Vec_insert_range(
    Vec *vec,
    const void *data,
    const size_t start_idx,
    const size_t quantity
);

int Vec_remove(Vec *vec, const size_t idx);
int Vec_remove_range(
    Vec *vec, const size_t from_idx, const size_t to_idx
);
int Vec_remove_all(Vec *vec);

int Vec_append(Vec *base, const Vec *vec);
int Vec_append_clone(Vec *vec1, Vec *vec2, Vec **new_vec);
int Vec_split(Vec *base, Vec **new_vec, const size_t idx);
int Vec_split_clone(
    const Vec *base,
    Vec **new_vec1,
    Vec **new_vec2,
    const size_t idx
);

int Vec_slice(Vec *vec, const size_t from_idx, const size_t to_idx);
Vec *Vec_slice_clone(
    const Vec *vec, const size_t from_idx, const size_t to_idx
);

static inline bool Vec_is_empty(Vec *vec) {
	if (vec->size == 0)
		return true;
	else
		return false;
}

int Vec_swap(Vec *vec, const size_t idx1, const size_t idx2);
int Vec_fill(Vec *vec, void *data);

#endif
