#include "../../ccoll-codes.h"
#include "../../colors.h"
#include "../../include/minivec.h"

#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>

MiniVec *MiniVec_init(size_t sizeof_element) {
	CCOLL_MINIVEC_ELEMENT_SIZE_CHECK_NULL(sizeof_element);
	// if (sizeof_element == 0) return NULL;
	// MINIVEC_ASSERT(
	//     sizeof_element <= alignof(max_align_t),
	//     "Check if realloc and malloc will handle alligment correctly"
	// );

	MiniVec *vec = (MiniVec *)malloc(sizeof(MiniVec));
	if (!vec) return NULL;

	vec->size		= 0;
	vec->element_size = sizeof_element;
	vec->capacity	= CCOLL_MINIVEC_MIN_CAPACITY;

	vec->data = (void *)malloc(vec->capacity * vec->element_size);
	if (!vec->data) {
		free(vec);
		return NULL;
	}

	return vec;
}

MiniVec *MiniVec_init_with(size_t sizeof_element, size_t min_capacity) {
	CCOLL_MINIVEC_ELEMENT_SIZE_CHECK_NULL(sizeof_element);
	// if (sizeof_element == 0) return NULL;
	// MINIVEC_ASSERT(
	//     sizeof_element <= alignof(max_align_t),
	//     "Check if realloc and malloc will handle alligment correctly"
	// );
	if (min_capacity == 0) return NULL;

	MiniVec *vec = (MiniVec *)malloc(sizeof(MiniVec));
	if (!vec) return NULL;

	vec->size		= 0;
	vec->element_size = sizeof_element;
	vec->capacity	= min_capacity;

	vec->data = (void *)malloc(vec->capacity * vec->element_size);
	if (!vec->data) {
		free(vec);
		return NULL;
	}

	return vec;
}
