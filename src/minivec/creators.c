#include "../../colors.h"
#include "../../include/minivec.h"

#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int _MiniVec_malloc(
    MiniVec **to_alloc, size_t item_size, size_t capacity, size_t alignment
) {
	CCOLL_MINIVEC_CHECK_ITEM_SIZE(item_size);
	CCOLL_MINIVEC_CHECK_CAPACITY(capacity, item_size);
	CCOLL_MINIVEC_CHECK_ALIGNMENT(alignment);

	MiniVec *vec = malloc(sizeof(MiniVec));
	if (!vec) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}

	vec->size	   = 0;
	vec->item_size = item_size;
	vec->capacity  = capacity;

	size_t padded_bytes =
	    MiniVec_pad_bytes_to_alignment(vec, capacity, alignment);

	if (padded_bytes < MiniVec_count_to_bytes(vec, capacity)) {
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
_MiniVec_init(size_t item_size, const struct _MiniVec_init_opts *opts) {

	size_t capacity  = CCOLL_MINIVEC_MIN_CAPACITY;
	size_t alignment = CCOLL_MINIVEC_DEFAULT_ALIGNMENT;

	if (opts) {
		if (opts->capacity) capacity = opts->capacity;
		if (opts->alignment) alignment = opts->alignment;
	}

	MiniVec *vec = NULL;
	if (_MiniVec_malloc(&vec, item_size, capacity, alignment)) {
		CCOLL_MINIVEC_ERROR("MiniVec init failed");
		return NULL;
	};

	return vec;
}
