#include "../../colors.h"
#include "../../include/vec.h"

#include <stddef.h>
#include <stdlib.h>

Vec *Vec_init(size_t size_of_data) {
	if (size_of_data == 0) return NULL;

	Vec *vec = (Vec *)malloc(sizeof(Vec));
	if (!vec) return NULL;

	vec->size		= 0;
	vec->element_size = size_of_data;
	vec->capacity	= VEC_MIN_CAPACITY;
	vec->after_rm	= NULL;

	vec->data = (void *)malloc(vec->capacity * vec->element_size);
	if (!vec->data) {
		free(vec);
		return NULL;
	}

	return vec;
}

Vec *Vec_init_with(size_t size_of_data, size_t min_capacity) {
	if (size_of_data == 0) return NULL;
	if (min_capacity == 0) return NULL;

	Vec *vec = (Vec *)malloc(sizeof(Vec));
	if (!vec) return NULL;

	vec->size		= 0;
	vec->element_size = size_of_data;
	vec->capacity	= min_capacity;
	vec->after_rm	= NULL;

	vec->data = (void *)malloc(vec->capacity * vec->element_size);
	if (!vec->data) {
		free(vec);
		return NULL;
	}

	return vec;
}
