#include "../../colors.h"
#include "../../include/minivec.h"

#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int _MiniVec_malloc(
    MiniVec **to_alloc, size_t sizeof_item, size_t capacity, size_t alignment
) {
	CCOLL_MINIVEC_ITEM_SIZE_CHECK_NULL(sizeof_item);

	if (capacity == 0) {
		CCOLL_MINIVEC_ERROR("Passed capacity of size 0");
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	if (MiniVec_mul_will_overflow(capacity, sizeof_item)) {
		CCOLL_MINIVEC_ERROR("bytes multiplication overflow");
		return CCOLL_OVERFLOW;
	}

	if (alignment < CCOLL_MINIVEC_MIN_ALIGNMENT) {
		CCOLL_MINIVEC_ERROR(
		    "passed aalignment is smaller than CCOLL_MINIVEC_MIN_ALIGNMENT"
		);
		return CCOLL_INVALID_ARGUMENT; // TODO: make err code for alignment
	}

	if ((alignment & (alignment - 1)) != 0) {
		CCOLL_MINIVEC_ERROR("passed alignment isn't pover of two (%zu)", capacity);
		return CCOLL_INVALID_ARGUMENT; // TODO: make err code for alignment
	}

	MiniVec *vec = malloc(sizeof(MiniVec));
	if (!vec) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}

	vec->size	   = 0;
	vec->item_size = sizeof_item;
	vec->capacity  = capacity;

	size_t bytes_needed = MiniVec_count_to_bytes(vec, capacity);
	size_t padded_bytes =
	    MiniVec_round_up_bytes_to_alignment(bytes_needed, alignment);

	if (padded_bytes < bytes_needed) {
		free(vec);
		CCOLL_MINIVEC_ERROR("size_t overflow on alignment calculation");
		return CCOLL_OVERFLOW;
	}

	vec->data = aligned_alloc(alignment, padded_bytes);
	if (!vec->data) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec data returned NULL");
		free(vec);
		return CCOLL_OUT_OF_MEMORY;
	}

	*to_alloc = vec;

	CCOLL_MINIVEC_LOG(
	    "operation successful, allocated space for %zu items with %zu "
	    "alignment (allocated %zu bytes)",
	    vec->capacity, alignment,
	    MiniVec_round_up_bytes_to_alignment(
		  MiniVec_count_to_bytes(vec, vec->capacity), alignment
	    )
	);
	return CCOLL_SUCCESS;
}

MiniVec *
_MiniVec_init(size_t sizeof_item, const struct _MiniVec_init_opts *opts) {

	size_t capacity  = CCOLL_MINIVEC_MIN_CAPACITY;
	size_t alignment = CCOLL_MINIVEC_DEFAULT_ALIGNMENT;

	if (opts) {
		if (opts->capacity) capacity = opts->capacity;
		if (opts->alignment) alignment = opts->alignment;
	}

	MiniVec *vec = NULL;
	if (_MiniVec_malloc(&vec, sizeof_item, capacity, alignment)) {
		CCOLL_MINIVEC_ERROR("MiniVec init failed");
		return NULL;
	};

	return vec;
}
