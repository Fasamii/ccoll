#include "../../ccoll-codes.h"
#include "../../colors.h"
#include "../../include/minivec.h"

#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

MiniVec *MiniVec_init(size_t sizeof_element) {
	CCOLL_MINIVEC_ELEMENT_SIZE_CHECK_NULL(sizeof_element);

	if (MiniVec_mul_will_overflow(
		  CCOLL_MINIVEC_MIN_CAPACITY, sizeof_element
	    )) {
		CCOLL_MINIVEC_ERROR("bytes multiplication overflow");
		return NULL;
	}

	MiniVec *vec = (MiniVec *)calloc(1, sizeof(MiniVec));
	if (!vec) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec failed");
		return NULL;
	}

	vec->size		= 0;
	vec->element_size = sizeof_element;
	vec->capacity	= CCOLL_MINIVEC_MIN_CAPACITY;

	vec->data = (void *)malloc(MiniVec_count_to_bytes(vec, vec->capacity));
	if (!vec->data) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec -> data failed");
		free(vec);
		return NULL;
	}

	CCOLL_MINIVEC_LOG("operation successful");
	return vec;
}

MiniVec *MiniVec_init_with(size_t sizeof_element, size_t min_capacity) {
	CCOLL_MINIVEC_ELEMENT_SIZE_CHECK_NULL(sizeof_element);
	if (min_capacity == 0) {
		CCOLL_MINIVEC_ERROR("Passed capacity of size 0");
		return NULL;
	}

	if (MiniVec_mul_will_overflow(min_capacity, sizeof_element)) {
		CCOLL_MINIVEC_ERROR("bytes multiplication overflow");
		return NULL;
	}

	MiniVec *vec = (MiniVec *)calloc(1, sizeof(MiniVec));
	if (!vec) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec failed");
		return NULL;
	}

	vec->size		= 0;
	vec->element_size = sizeof_element;
	vec->capacity	= min_capacity;

	vec->data = (void *)malloc(MiniVec_count_to_bytes(vec, vec->capacity));
	if (!vec->data) {
		CCOLL_MINIVEC_ERROR("malloc for MiniVec -> data failed");
		free(vec);
		return NULL;
	}

	CCOLL_MINIVEC_LOG("operation successful");
	return vec;
}
