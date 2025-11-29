#include "../../include/vec.h"
#include "./checks.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline int
_Vec_internal_realloc(Vec *vec, size_t capacity, size_t alignment) {
	CCOLL_VEC_CHECK_VEC_INTEGRITY(vec);
	CCOLL_VEC_CHECK_CAPACITY(capacity, vec->item_size);
	CCOLL_VEC_CHECK_ALIGNMENT(alignment);

	if (capacity == vec->capacity) {
		CCOLL_LOG("nothing to do returning SUCCESS");
		return CCOLL_SUCCESS;
	}

	size_t bytes_needed = Vec_count_to_bytes(vec, capacity);
	size_t padded_bytes =
	    Vec_round_up_bytes_to_alignment(bytes_needed, alignment);

	if (padded_bytes < bytes_needed) {
		CCOLL_ERROR("size_t overflow on alignment calculation");
		return CCOLL_OVERFLOW;
	}

	void *tmp_data = aligned_alloc(alignment, padded_bytes);
	if (!tmp_data) {
		CCOLL_ERROR("realloc returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}

	if (Vec_mul_will_overflow(vec->size, vec->item_size)) {
		CCOLL_ERROR("bytes multiplication overflow");
		free(tmp_data);
		return CCOLL_OVERFLOW;
	}

	memcpy(tmp_data, vec->data, Vec_count_to_bytes(vec, vec->size));

	free(vec->data);
	vec->data = NULL;

	vec->data	  = tmp_data;
	vec->capacity = capacity;

	CCOLL_LOG(
	    "changed capacity from (%zu) to (%zu)", vec->capacity, capacity
	);

	return CCOLL_SUCCESS;
}

static inline int _Vec_grow_capacity_to_fit(
    size_t *capacity, CCOLL_GROWTH_STRATEGY growth_strategy
) {
	if (capacity == NULL) return CCOLL_NULL;

	if (growth_strategy == CCOLL_GROWTH_STRATEGY_EXACTLY)
		return CCOLL_SUCCESS;

	size_t new_capacity = 1;
	switch (growth_strategy) {
	case CCOLL_GROWTH_STRATEGY_GOLDEN:
		if (*capacity > (SIZE_MAX - 999) / 1618) {
			return CCOLL_OVERFLOW;
		}
		*capacity = (*capacity * 1618 + 999) / 1000;
		break;
	case CCOLL_GROWTH_STRATEGY_DOUBLE:
		while (new_capacity < *capacity) {
			if (new_capacity > SIZE_MAX / 2) return CCOLL_OVERFLOW;
			new_capacity *= 2;
		}
		if (new_capacity < CCOLL_VEC_MIN_CAPACITY) {
			*capacity = CCOLL_VEC_MIN_CAPACITY;
		} else {
			*capacity = new_capacity;
		}
		break;
	default:
		CCOLL_ERROR(
		    "definied not valid growth strategy (use enum definied in "
		    "ccoll_codes.h file)"
		);
		return CCOLL_INVALID_ARGUMENT;
	}

	return CCOLL_SUCCESS;
}

int _Vec_change_capacity(
    Vec *vec,
    const size_t capacity,
    const struct _Vec_change_capacity_opts *opts
) {
	CCOLL_VEC_CHECK_VEC_INTEGRITY(vec);

	size_t alignment = CCOLL_VEC_DEFAULT_ALIGNMENT;

	if (opts) {
		if (opts->alignment) alignment = opts->alignment;
	}

	if (capacity < CCOLL_VEC_MIN_CAPACITY) {
		CCOLL_WARN(
		    "requested less memory than CCOLL_VEC_MIN_CAPACITY allows"
		);
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	int res = 0;
	if ((res = _Vec_internal_realloc(vec, capacity, alignment))) return res;

	CCOLL_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int _Vec_alloc(
    Vec *vec,
    const size_t idxs,
    const struct _Vec_growth_capacity_opts *opts
) {
	CCOLL_VEC_CHECK_VEC_INTEGRITY(vec);
	CCOLL_VEC_CHECK_OVERFLOW_ADD(vec->capacity, idxs);

	size_t alignment	     = CCOLL_VEC_DEFAULT_ALIGNMENT;
	size_t growth_strategy = CCOLL_VEC_GROWTH_STRATEGY;

	if (opts) {
		if (opts->alignment) alignment = opts->alignment;
		if (opts->growth_strategy) growth_strategy = opts->growth_strategy;
	}

	size_t new_capacity = vec->capacity + idxs;

	if (new_capacity < CCOLL_VEC_MIN_CAPACITY) {
		CCOLL_WARN(
		    "requested less memory than CCOLL_VEC_MIN_CAPACITY allows"
		);
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	int res = 0;

	if ((res =
		   _Vec_grow_capacity_to_fit(&new_capacity, growth_strategy))) {
		return res;
	}

	if ((res = _Vec_internal_realloc(vec, new_capacity, alignment))) {
		return res;
	}

	CCOLL_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int _Vec_reserve_additional(
    Vec *vec,
    const size_t idxs,
    const struct _Vec_growth_capacity_opts *opts
) {
	CCOLL_VEC_CHECK_VEC_INTEGRITY(vec);
	CCOLL_VEC_CHECK_OVERFLOW_ADD(vec->size, idxs);

	size_t alignment				  = CCOLL_VEC_DEFAULT_ALIGNMENT;
	CCOLL_GROWTH_STRATEGY growth_strategy = CCOLL_VEC_GROWTH_STRATEGY;

	if (opts) {
		if (opts->alignment) alignment = opts->alignment;
		if (opts->growth_strategy) growth_strategy = opts->growth_strategy;
	}

	size_t new_capacity = vec->size + idxs;
	if (vec->capacity >= new_capacity) {
		CCOLL_LOG("nothing to do returning SUCCESS");
		return CCOLL_SUCCESS;
	}

	int res = 0;

	if ((res =
		   _Vec_grow_capacity_to_fit(&new_capacity, growth_strategy))) {
		return res;
	}

	if ((res = _Vec_internal_realloc(vec, new_capacity, alignment))) {
		return res;
	}

	CCOLL_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int _Vec_shrink(
    Vec *vec, const struct _Vec_change_capacity_opts *opts
) {
	CCOLL_VEC_CHECK_VEC_INTEGRITY(vec);

	if (vec->size == vec->capacity) {
		CCOLL_LOG("nothing to do returning SUCCESS");
		return CCOLL_SUCCESS;
	}

	size_t alignment = CCOLL_VEC_DEFAULT_ALIGNMENT;

	if (opts) {
		if (opts->alignment) alignment = opts->alignment;
	}

	size_t new_capacity;
	if (vec->size < CCOLL_VEC_MIN_CAPACITY) {
		new_capacity = CCOLL_VEC_MIN_CAPACITY;
	} else {
		new_capacity = vec->size;
	}

	int res = 0;
	if ((res = _Vec_internal_realloc(vec, new_capacity, alignment))) return res;

	CCOLL_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int _Vec_free(Vec *vec) {
	CCOLL_VEC_CHECK_VEC_INTEGRITY(vec);

	free(vec->data);
	free(vec);

	return CCOLL_SUCCESS;
}
