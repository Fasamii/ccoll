#include "../../include/vec.h"
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

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

// TODO: FIX: find better way to handle errors in destructor for that foo
// make it more recoverable and return more data about error e.g.: what was
// actually freed. Or return Vec with indexes where destructor failed, think
// about that
// TODO: make that fallback to free(vec->data[idx]) if no destructor foo
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

	if (errors_count > 0) return CCOLL_DESTRUCTOR_FOO_FAIL_CONTINUED;
	return CCOLL_SUCCESS;
}

// TODO:TEST: that foo
// TODO:FIX: handle destructor errors better
int Vec_free_range(Vec *vec, size_t from_idx, size_t to_idx) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (from_idx > to_idx) return CCOLL_INVALID_ARGUMENT;
	if (to_idx > vec->size) return CCOLL_INVALID_ARGUMENT;

	int errors = 0;
	if (vec->destructor) {
		for (size_t i = to_idx; i >= from_idx; i--) {
			if (vec->destructor(vec->data + (i * vec->element_size)))
				errors++;
		}
	}

	memmove(
	    vec->data + (from_idx * vec->element_size),
	    vec->data + (to_idx * vec->element_size),
	    (vec->size - to_idx) * vec->element_size
	);

	vec->size -= (to_idx - from_idx);

	if (errors) return CCOLL_DESTRUCTOR_FOO_FAIL_CONTINUED;
	return CCOLL_SUCCESS;
}

// TODO:FIX: handle destructor errors better
// TODO:TEST: that foo
int Vec_free_element(Vec *vec, size_t idx) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (idx >= vec->size) return CCOLL_INVALID_ARGUMENT;

	if (vec->destructor) {
		if (vec->destructor(vec->data + (idx * vec->element_size)))
			return CCOLL_DESTRUCTOR_FOO_FAIL;
	}

	memmove(
	    vec->data + (idx * vec->element_size),
	    vec->data + ((idx + 1) * vec->element_size),
	    (vec->size - idx - 1) * vec->element_size
	);

	vec->size--;

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

	vec->data	  = tmp_data;
	vec->capacity = idxs;

	return CCOLL_SUCCESS;
}
