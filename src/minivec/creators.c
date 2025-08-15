#include "../../colors.h"
#include "../../include/minivec.h"

#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

int _MiniVec_malloc(MiniVec **to_alloc, size_t sizeof_item, size_t capacity) {
	CCOLL_MINIVEC_ITEM_SIZE_CHECK_NULL(sizeof_item);

	if (capacity == 0) {
		CCOLL_MINIVEC_ERROR("Passed capacity of size 0");
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	if (MiniVec_mul_will_overflow(capacity, sizeof_item)) {
		CCOLL_MINIVEC_ERROR("bytes multiplication overflow");
		return CCOLL_OVERFLOW;
	}

	MiniVec *vec = (MiniVec *)malloc(sizeof(MiniVec));
	if (!vec) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}

	vec->size	   = 0;
	vec->item_size = sizeof_item;
	vec->capacity  = capacity;

	vec->data = (void *)malloc(MiniVec_count_to_bytes(vec, capacity));
	if (!vec->data) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec data returned NULL");
		free(vec);
		return CCOLL_OUT_OF_MEMORY;
	}

	*to_alloc = vec;

	return CCOLL_SUCCESS;
}

MiniVec *_MiniVec_init(size_t sizeof_item, const struct _MiniVec_init_opts *opts) {

	size_t capacity = CCOLL_MINIVEC_MIN_CAPACITY;

	if (opts) {
		if (opts->capacity) capacity = opts->capacity;
	}

	MiniVec *vec = NULL;
	if (_MiniVec_malloc(&vec, sizeof_item, capacity)) {
		CCOLL_MINIVEC_LOG("MiniVec init failed");
		return NULL;
	};

	CCOLL_MINIVEC_LOG("operation successful");
	return vec;
}
