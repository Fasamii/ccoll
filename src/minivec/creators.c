#include "../../ccoll-codes.h"
#include "../../colors.h"
#include "../../include/minivec.h"

#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

int _MiniVec_malloc(
    MiniVec **to_alloc, size_t sizeof_element, size_t capacity
) {
	CCOLL_MINIVEC_ELEMENT_SIZE_CHECK_NULL(sizeof_element);

	if (capacity == 0) {
		CCOLL_MINIVEC_ERROR("Passed capacity of size 0");
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;
	}

	if (MiniVec_mul_will_overflow(capacity, sizeof_element)) {
		CCOLL_MINIVEC_ERROR("bytes multiplication overflow");
		return CCOLL_OVERFLOW;
	}

	MiniVec *vec = (MiniVec *)malloc(sizeof(MiniVec));
	if (!vec) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec returned NULL");
		return CCOLL_OUT_OF_MEMORY;
	}

	vec->size		= 0;
	vec->element_size = sizeof_element;
	vec->capacity	= capacity;

	vec->data = (void *)malloc(MiniVec_count_to_bytes(vec, capacity));
	if (!vec->data) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec data returned NULL");
		free(vec);
		return CCOLL_OUT_OF_MEMORY;
	}

	*to_alloc = vec;

	return CCOLL_SUCCESS;
}

MiniVec *MiniVec_init(size_t sizeof_element) {

	int ret	 = 0;
	MiniVec *vec = NULL;

	ret = _MiniVec_malloc(&vec, sizeof_element, CCOLL_MINIVEC_MIN_CAPACITY);

	if (ret != CCOLL_SUCCESS) {
		CCOLL_MINIVEC_LOG("MiniVec init failed");
		return NULL;
	} else {
		CCOLL_MINIVEC_LOG("operation successful");
		return vec;
	}
}

MiniVec *MiniVec_init_with(size_t sizeof_element, size_t capacity) {

	int ret	 = 0;
	MiniVec *vec = NULL;

	ret = _MiniVec_malloc(&vec, sizeof_element, capacity);

	if (ret != CCOLL_SUCCESS) {
		CCOLL_MINIVEC_LOG("MiniVec init with capacity failed");
		return NULL;
	} else {
		CCOLL_MINIVEC_LOG("operation successful");
		return vec;
	}
}
