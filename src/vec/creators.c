#include "../../include/vec.h"
#include "./checks.h"

#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int _Vec_internal_malloc(
    Vec **to_alloc, size_t item_size, size_t capacity, size_t alignment
) {
	CCOLL_VEC_CHECK_ITEM_SIZE(item_size);
	CCOLL_VEC_CHECK_CAPACITY(capacity, item_size);
	CCOLL_VEC_CHECK_ALIGNMENT(alignment);

	Vec *vec = malloc(sizeof(Vec));
	if (!vec) {
		CCOLL_ERROR("malloc for Vec returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}

	vec->size	   = 0;
	vec->item_size = item_size;
	vec->capacity  = capacity;

	size_t padded_bytes =
	    Vec_pad_bytes_to_alignment(vec, capacity, alignment);

	if (padded_bytes < Vec_count_to_bytes(vec, capacity)) {
		free(vec);
		CCOLL_ERROR("size_t overflow on alignment calculation");
		return CCOLL_OVERFLOW;
	}

	vec->data = aligned_alloc(alignment, padded_bytes);
	if (!vec->data) {
		CCOLL_ERROR("malloc for Vec data returned NULL");
		free(vec);
		return CCOLL_OUT_OF_MEMORY;
	}

	*to_alloc = vec;

	CCOLL_LOG(
	    "operation successful, allocated space for %zu items with %zu "
	    "alignment (allocated %zu bytes)",
	    vec->capacity, alignment,
	    Vec_round_up_bytes_to_alignment(
		  Vec_count_to_bytes(vec, vec->capacity), alignment
	    )
	);
	return CCOLL_SUCCESS;
}

Vec *
_Vec_init(size_t item_size, const struct _Vec_init_opts *opts) {

	size_t capacity  = CCOLL_VEC_MIN_CAPACITY;
	size_t alignment = CCOLL_VEC_DEFAULT_ALIGNMENT;

	if (opts) {
		if (opts->capacity) capacity = opts->capacity;
		if (opts->alignment) alignment = opts->alignment;
	}

	Vec *vec = NULL;
	if (_Vec_internal_malloc(&vec, item_size, capacity, alignment)) {
		CCOLL_ERROR("Vec init failed");
		return NULL;
	};

	return vec;
}
