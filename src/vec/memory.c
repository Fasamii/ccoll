#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../ccoll-codes.h"
#include "../../include/vec.h"

int Vec_reserve(Vec *vec, const size_t idxs) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;

	size_t toalloc = vec->size + idxs;
	if (toalloc <= vec->capacity) return CCOLL_SUCCESS;

	void *tmp_data =
	    (void *)realloc(vec->data, (toalloc * vec->element_size));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data	  = tmp_data;
	vec->capacity = toalloc;

	return CCOLL_SUCCESS;
}

int Vec_reserve_entire(Vec *vec, const size_t idxs) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (idxs < vec->size) return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;

	if (idxs <= vec->capacity) return CCOLL_SUCCESS;

	void *tmp_data = realloc(vec->data, Vec_idx_to_bytes(vec, idxs));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;
	vec->data	  = tmp_data;
	vec->capacity = idxs;

	return CCOLL_SUCCESS;
}

int Vec_alloc(Vec *vec, const size_t idxs) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;

	void *tmp_data =
	    realloc(vec->data, Vec_idx_to_bytes(vec, vec->capacity + idxs));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data = tmp_data;
	vec->capacity += idxs;

	return CCOLL_SUCCESS;
}

int Vec_free(Vec *vec) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;

	if (vec->on_change) {
		for (size_t i = 0; i < vec->size; i++) {
			vec->on_change(
			    Vec_get_unchecked_ptr(vec, i), i, vec->element_size,
			    CCOLL_OPERATION_REMOVE_FORCED
			);
		}
	}

	free(vec->data);
	free(vec);

	return CCOLL_SUCCESS;
}

int Vec_free_ammount(Vec *vec, size_t idxs) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if ((vec->capacity - vec->size) < idxs)
		return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;

	void *tmp_data =
	    realloc(vec->data, Vec_idx_to_bytes(vec, vec->capacity - idxs));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;
	vec->data = tmp_data;
	vec->capacity -= idxs;

	return CCOLL_SUCCESS;
}

int Vec_shrink(Vec *vec) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;

	void *tmp_data = realloc(vec->data, (vec->size * vec->element_size));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;
	vec->data	  = tmp_data;
	vec->capacity = vec->size;

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
int Vec_shrink_to(Vec *vec, const size_t idxs) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (idxs < vec->size) return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;

	void *tmp_data = realloc(vec->data, idxs * vec->element_size);
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data	  = tmp_data;
	vec->capacity = idxs;

	return CCOLL_SUCCESS;
}
