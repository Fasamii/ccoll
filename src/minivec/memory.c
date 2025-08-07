#include "../../ccoll-codes.h"
#include "../../colors.h"
#include "../../include/minivec.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline int _MiniVec_realloc(MiniVec *vec, size_t capacity) {
	if (capacity == 0) {
		CCOLL_MINIVEC_WARN("cannot request capacity of size 0");
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	if (MiniVec_mul_will_overflow(capacity, vec->element_size)) {
		CCOLL_MINIVEC_ERROR("bytes multiplication overflow");
		return CCOLL_OVERFLOW;
	}

	if (capacity < CCOLL_MINIVEC_MIN_CAPACITY) {
		CCOLL_MINIVEC_WARN(
		    "requested less memory than CCOLL_MINIVEC_MIN_CAPACITY allows"
		);
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	if (capacity == vec->capacity) {
		CCOLL_MINIVEC_LOG("nothing to do returning SUCCESS");
		return CCOLL_SUCCESS;
	}

	if (capacity < vec->size) {
		CCOLL_MINIVEC_WARN("Requested not enough memory to store vec size");
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	void *tmp_data =
	    realloc(vec->data, MiniVec_count_to_bytes(vec, capacity));
	if (!tmp_data) {
		CCOLL_MINIVEC_ERROR("realloc returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}

	CCOLL_MINIVEC_LOG(
	    "changed capacity from (%zu) to (%zu)", vec->capacity, capacity
	);

	vec->data	  = tmp_data;
	vec->capacity = capacity;

	return CCOLL_SUCCESS;
}

static inline int _MiniVec_apply_growth_strategy_to_capacity(size_t *capacity) {
	return CCOLL_SUCCESS;
}

int MiniVec_change_capacity(MiniVec *vec, const size_t capacity) {
	CCOLL_MINIVEC_INTEGRITY_CHECK(vec);

	int res = 0;
	if ((res = _MiniVec_realloc(vec, capacity))) return res;

	CCOLL_MINIVEC_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int MiniVec_alloc(MiniVec *vec, const size_t idxs) {
	CCOLL_MINIVEC_INTEGRITY_CHECK(vec);

	size_t new_capacity = vec->capacity + idxs;

	int res = 0;

	if ((res = _MiniVec_apply_growth_strategy_to_capacity(&new_capacity))) {
		return res;
	}

	if ((res = _MiniVec_realloc(vec, new_capacity))) {
		return res;
	}

	CCOLL_MINIVEC_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int MiniVec_reserve_additional(MiniVec *vec, const size_t idxs) {
	CCOLL_MINIVEC_INTEGRITY_CHECK(vec);

	size_t new_capacity = vec->size + idxs;
	if (vec->capacity >= new_capacity) {
		CCOLL_MINIVEC_LOG("nothing to do returning SUCCESS");
		return CCOLL_SUCCESS;
	}

	int res = 0;

	if ((res = _MiniVec_apply_growth_strategy_to_capacity(&new_capacity))) {
		return res;
	}

	if ((res = _MiniVec_realloc(vec, new_capacity))) {
		return res;
	}

	CCOLL_MINIVEC_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int MiniVec_shrink(MiniVec *vec) {
	CCOLL_MINIVEC_INTEGRITY_CHECK(vec);

	if (vec->size == vec->capacity) {
		CCOLL_MINIVEC_LOG("nothing to do returning SUCCESS");
		return CCOLL_SUCCESS;
	}

	size_t new_capacity;
	if (vec->size < CCOLL_MINIVEC_MIN_CAPACITY) {
		new_capacity = CCOLL_MINIVEC_MIN_CAPACITY;
	} else {
		new_capacity = vec->size;
	}

	int res = 0;
	if ((res = _MiniVec_realloc(vec, new_capacity))) return res;

	CCOLL_MINIVEC_LOG("operation successful");
	return CCOLL_SUCCESS;
}
