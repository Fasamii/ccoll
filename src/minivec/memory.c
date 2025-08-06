#include "../../ccoll-codes.h"
#include "../../colors.h"
#include "../../include/minivec.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int MiniVec_reserve(MiniVec *vec, const size_t idxs) {
	CCOLL_MINIVEC_INTEGRITY_CHECK(vec);

	if (idxs == 0) {
		MINIVEC_WARN("requested size is 0 - nothing to do, returngin SUCCESS");
		return CCOLL_SUCCESS;
	}
	if (SIZE_MAX - vec->size < idxs) {
		MINIVEC_ERROR("overflow");
		return CCOLL_OVERFLOW;
	}

	size_t to_alloc = vec->size + idxs;
	if (vec->capacity >= to_alloc) {
		MINIVEC_LOG("nothing to do returngin SUCCESS");
		return CCOLL_SUCCESS;
	}

	if (MiniVec_mul_will_overflow(to_alloc, vec->element_size)) {
		MINIVEC_ERROR("bytes multiplication overflow");
		return CCOLL_OVERFLOW;
	}

	void *tmp_data =
	    realloc(vec->data, MiniVec_count_to_bytes(vec, to_alloc));
	if (!tmp_data) {
		MINIVEC_ERROR("realloc returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}
	vec->data	  = tmp_data;
	vec->capacity = to_alloc;

	MINIVEC_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int MiniVec_alloc(MiniVec *vec, const size_t idxs) {
	CCOLL_MINIVEC_INTEGRITY_CHECK(vec);

	if (idxs == 0) {
		MINIVEC_WARN("requested size is 0 - nothing to do, returngin SUCCESS");
		return CCOLL_SUCCESS;
	}
	if (SIZE_MAX - vec->capacity < idxs) {
		MINIVEC_ERROR("overflow");
		return CCOLL_OVERFLOW;
	}

	size_t new_capacity = vec->capacity + idxs;

	if (MiniVec_mul_will_overflow(new_capacity, vec->element_size)) {
		MINIVEC_ERROR("overflow");
		return CCOLL_OVERFLOW;
	}

	void *tmp_data =
	    realloc(vec->data, MiniVec_count_to_bytes(vec, new_capacity));
	if (!tmp_data) {
		MINIVEC_ERROR("realloc returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}
	vec->data	  = tmp_data;
	vec->capacity = new_capacity;

	MINIVEC_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int MiniVec_shrink(MiniVec *vec) {
	CCOLL_MINIVEC_INTEGRITY_CHECK(vec);

	if (vec->size == vec->capacity) {
		MINIVEC_LOG("nothing to do returning SUCCESS");
		return CCOLL_SUCCESS;
	}

	size_t new_capacity = vec->size;
	if (new_capacity < CCOLL_MINIVEC_MIN_CAPACITY) {
		MINIVEC_WARN(
		    "Requested less memory than CCOLL_MINIVEC_MIN_CAPACITY allows"
		);
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	if (MiniVec_mul_will_overflow(new_capacity, vec->element_size)) {
		MINIVEC_ERROR("bytes multiplication overflow");
		return CCOLL_OVERFLOW;
	}

	void *tmp_data =
	    realloc(vec->data, MiniVec_count_to_bytes(vec, new_capacity));
	if (!tmp_data) {
		MINIVEC_ERROR("realloc returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}

	vec->capacity = new_capacity;
	vec->data	  = tmp_data;

	MINIVEC_LOG("operation successful");
	return CCOLL_SUCCESS;
}

int MiniVec_change_capacity(MiniVec *vec, const size_t capacity) {
	CCOLL_MINIVEC_INTEGRITY_CHECK(vec);

	if (capacity < CCOLL_MINIVEC_MIN_CAPACITY) {
		MINIVEC_WARN(
		    "Requested less memory than CCOLL_MINIVEC_MIN_CAPACITY allows"
		);
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	if (capacity < vec->size) {
		MINIVEC_WARN("Requested not enough memory to store vec size");
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	if (capacity == vec->capacity) {
		MINIVEC_LOG("nothing to do returngin SUCCESS");
		return CCOLL_SUCCESS;
	}

	if (MiniVec_mul_will_overflow(capacity, vec->element_size)) {
		MINIVEC_ERROR("bytes multiplication overflow");
		return CCOLL_OVERFLOW;
	}

	void *tmp_data =
	    realloc(vec->data, MiniVec_count_to_bytes(vec, capacity));
	if (!tmp_data) {
		MINIVEC_ERROR("realloc returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}
	vec->data	  = tmp_data;
	vec->capacity = capacity;

	MINIVEC_LOG("operation successful");
	return CCOLL_SUCCESS;
}
