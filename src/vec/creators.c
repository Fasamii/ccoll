#include "../../ccoll_errors.h"
#include "../../colors.h"
#include "../../include/vec.h"

#include <stddef.h>
#include <stdlib.h>

Vec *Vec_init(const size_t size_of_data) {
	if (size_of_data == 0) return NULL;

	Vec *vec = (Vec *)malloc(sizeof(Vec));
	if (!vec) return NULL;

	vec->size		= 0;
	vec->element_size = size_of_data;
	vec->capacity	= CCOLL_VEC_MIN_CAPACITY;
	vec->on_remove	= NULL;

	vec->data = (void *)malloc(vec->capacity * vec->element_size);
	if (!vec->data) {
		free(vec);
		return NULL;
	}

	return vec;
}

Vec *Vec_init_with(const size_t size_of_data, const size_t min_capacity) {
	if (size_of_data == 0) return NULL;
	if (min_capacity == 0) return NULL;

	Vec *vec = (Vec *)malloc(sizeof(Vec));
	if (!vec) return NULL;

	vec->size		= 0;
	vec->element_size = size_of_data;
	vec->capacity	= min_capacity;
	vec->on_remove	= NULL;

	vec->data = (void *)malloc(vec->capacity * vec->element_size);
	if (!vec->data) {
		free(vec);
		return NULL;
	}

	return vec;
}

// TODO: think about making struct alloc size without fn pointer and in that foo
// realloc if then you could include some other useful foo's without growing vec
// struct size when unneeded
int Vec_set_on_remove_callback(
    Vec *vec,
    int (*fn)(
	  void *, size_t idx, size_t element_size, CCOLL_OPERATION operation
    )
) {
	if (!vec) return CCOLL_NULL;
	if (!fn) return CCOLL_NULL_FN;
	vec->on_remove = fn;
	return CCOLL_SUCCESS;
}
