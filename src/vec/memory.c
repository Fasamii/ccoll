#include "../../include/vec.h"
#include <stdlib.h>

int Vec_reserve(Vec *vec, size_t idxs) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;

	size_t toalloc = vec->size + idxs;
	if (toalloc <= vec->capacity) return CCOLL_SUCCESS; 

	void *tmp_data = realloc(vec->data, (toalloc * vec->element_size));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data	  = tmp_data;
	vec->capacity = toalloc;

	return CCOLL_SUCCESS;
}

int Vec_alloc(Vec *vec, size_t idxs) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;

	void *tmp_data = (void *
	)realloc(vec->data, (vec->capacity + idxs) * vec->element_size);
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data = tmp_data;
	vec->capacity += idxs;

	return CCOLL_SUCCESS;
}

int Vec_free_shallow(Vec *vec) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;

	free(vec->data);
	free(vec);

	return CCOLL_SUCCESS;
}

int Vec_free(Vec *vec) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!vec->destructor) return CCOLL_INVALID_ARGUMENT;

	size_t errors_count = 0;
	for (size_t i = 0; i < vec->size; i++) {
		if (vec->destructor(vec->data + (i * vec->element_size)))
			errors_count++;
	}

	free(vec->data);
	free(vec);

	if (errors_count > 0) return CCOLL_FREED_WITH_ERRORS;
	return CCOLL_SUCCESS;
}

int Vec_shrink(Vec *vec) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;

	void *tmp_data = realloc(vec->data, (vec->size * vec->element_size));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;
	vec->data	  = tmp_data;
	vec->capacity = vec->size;

	return CCOLL_SUCCESS;
}

int Vec_shrink_to(Vec *vec, size_t idxs) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (idxs < vec->size) return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;

	void *tmp_data = realloc(vec->data, idxs * vec->element_size);
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data = tmp_data;
	vec->capacity = idxs;

	return CCOLL_SUCCESS;
}
