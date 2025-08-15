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

#define MiniVec_count_to_bytes(vec, idxs) ((vec)->item_size * (idxs))
#define MiniVec_bytes_to_count(vec, bytes) ((bytes) / (vec)->item_size)
#define MiniVec_mul_will_overflow(a, b) ((a) != 0 && (b) > SIZE_MAX / (a))
#define MiniVec_round_up_bytes_to_alignment(size, alignment)                   \
	(((size) + (alignment) - 1) / (alignment) * (alignment))

// TODO: make docs
MiniVec *
_MiniVec_init(size_t sizeof_item, const struct _MiniVec_init_opts *opts);
// TODO: make docs
#define MiniVec_init(sizeof_item, ...)                                         \
	_MiniVec_init(                                                           \
	    sizeof_item, &(const struct _MiniVec_init_opts){__VA_ARGS__}         \
	)

// TODO: make docs
int _MiniVec_free(MiniVec *vec);
// TODO: make docs
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
int _MiniVec_shrink(MiniVec *vec, const struct _MiniVec_change_capacity_opts *opts);
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

#if CCOLL_MINIVEC_DEBUG

#include <stdio.h>

#include "../colors.h"

#define CCOLL_MINIVEC_LOG(msg, ...)                                            \
	do {                                                                     \
		fprintf(                                                           \
		    stdout,                                                        \
		    BLU "┌[ " RED "!" BLU " ]" BLU " LOG::[" NOCOL "MiniVec" BLU   \
			  "]:[" MAG "FOO " NOCOL "%s()" BLU "]:[" MAG "FILE " NOCOL  \
			  "%s" BLU "]:[" MAG "LINE " NOCOL "%d" BLU "]" NOCOL "\n",  \
		    __func__, __FILE__, __LINE__                                   \
		);                                                                 \
		fprintf(                                                           \
		    stdout, BLU "└[msg]: " NOCOL msg NOCOL "\n\n", ##__VA_ARGS__   \
		);                                                                 \
	} while (0)
#define CCOLL_MINIVEC_WARN(msg, ...)                                           \
	do {                                                                     \
		fprintf(                                                           \
		    stdout,                                                        \
		    YEL "┌[ " RED "!!" YEL " ]" BLU " WARN"                        \
			  "::[" NOCOL "MiniVec" BLU "]:[" MAG "FOO " NOCOL           \
			  "%s()" BLU "]:[" MAG "FILE " NOCOL "%s" BLU "]:[" MAG      \
			  "LINE " NOCOL "%d" BLU "]" NOCOL "\n",                     \
		    __func__, __FILE__, __LINE__                                   \
		);                                                                 \
		fprintf(                                                           \
		    stdout, YEL "└[msg]: " NOCOL msg NOCOL "\n\n", ##__VA_ARGS__   \
		);                                                                 \
	} while (0)
#define CCOLL_MINIVEC_ERROR(msg, ...)                                          \
	do {                                                                     \
		fprintf(                                                           \
		    stdout,                                                        \
		    RED "┌[ " RED "!!!" RED " ]" BLU " ERROR::[" NOCOL             \
			  "MiniVec" BLU "]:[" MAG "FOO " NOCOL "%s()" BLU "]:[" MAG  \
			  "FILE " NOCOL "%s" BLU "]:[" MAG "LINE " NOCOL "%d" BLU    \
			  "]" NOCOL "\n",                                            \
		    __func__, __FILE__, __LINE__                                   \
		);                                                                 \
		fprintf(                                                           \
		    stdout, RED "└[msg]: " NOCOL msg NOCOL "\n\n", ##__VA_ARGS__   \
		);                                                                 \
	} while (0)
#define CCOLL_MINIVEC_ASSERT(CONDITION, msg, ...)                              \
	do {                                                                     \
		if (!(CONDITION)) {                                                \
			fprintf(                                                     \
			    stderr,                                                  \
			    RED "┌[///// [ASSERT FAIL] /////]" BLU ":[" NOCOL        \
				  "MiniVec" BLU "]:[" MAG "FOO " NOCOL "%s" BLU        \
				  "]:[" MAG "FILE " NOCOL "%s" BLU "]:[" MAG           \
				  "LINE " NOCOL "%d" BLU "]" NOCOL "\n",               \
			    __func__, __FILE__, __LINE__                             \
			);                                                           \
			fprintf(                                                     \
			    stderr, RED "└[msg]: " NOCOL msg NOCOL "\n\n",           \
			    ##__VA_ARGS__                                            \
			);                                                           \
		} else {                                                           \
			fprintf(                                                     \
			    stdout,                                                  \
			    BLU "┌[" GRN "ASSERT SUCCESS" BLU "]::[" NOCOL           \
				  "MiniVec" BLU "]::[" MAG "FOO " NOCOL "%s" BLU       \
				  "]::[" MAG "FILE " NOCOL "%s" BLU "]::[" MAG         \
				  "LINE " NOCOL "%d" BLU "]" NOCOL "\n",               \
			    __func__, __FILE__, __LINE__                             \
			);                                                           \
			fprintf(                                                     \
			    stdout, BLU "└[msg]: " NOCOL msg NOCOL "\n\n",           \
			    ##__VA_ARGS__                                            \
			);                                                           \
		}                                                                  \
	} while (0)
#else
#define CCOLL_MINIVEC_LOG(msg, ...)                                            \
	do {                                                                     \
	} while (0)

#define CCOLL_MINIVEC_WARN(msg, ...)                                           \
	do {                                                                     \
	} while (0)
#define CCOLL_MINIVEC_ERROR(msg, ...)                                          \
	do {                                                                     \
	} while (0)

#define CCOLL_MINIVEC_ASSERT(CONDITION, msg, ...)                              \
	do {                                                                     \
	} while (0)

#endif

#if CCOLL_MINIVEC_ARG_CHECK
#define CCOLL_MINIVEC_INTEGRITY_CHECK(ptr)                                     \
	if (!(ptr)) {                                                            \
		CCOLL_MINIVEC_ERROR("passed MiniVec is NULl");                     \
		return CCOLL_NULL;                                                 \
	};                                                                       \
	if (!(ptr)->data) {                                                      \
		CCOLL_MINIVEC_ERROR("data of passed MiniVec is NULL");             \
		return CCOLL_NULL_INTERNAL_DATA;                                   \
	}

#define CCOLL_MINIVEC_ITEM_SIZE_CHECK_NULL(sizeof_item)                        \
	if (((sizeof_item) == 0)) {                                              \
		CCOLL_MINIVEC_ERROR("element size 0 is not allowed");              \
		return CCOLL_INVALID_ARGUMENT;                                     \
	}

#define CCOLL_MINIVEC_CHECK_OVERFLOW_ADD(a, b)                                 \
	if (SIZE_MAX - a < b) {                                                  \
		CCOLL_MINIVEC_ERROR("overflow");                                   \
		return CCOLL_OVERFLOW;                                             \
	}
#else
#define CCOLL_MINIVEC_INTEGRITY_CHECK(ptr)
#define CCOLL_MINIVEC_ITEM_SIZE_CHECK_NULL(sizeof_item)
#define CCOLL_MINIVEC_CHECK_OVERFLOW_ADD(a, b)
#endif

#endif
