#include "../../ccoll_errors.h"
#include "../../include/vec.h"
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// TODO:TEST: Make test for that foo
int Vec_reserve(Vec *vec, const size_t idxs) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;

	size_t toalloc = vec->size + idxs;
	if (toalloc <= vec->capacity) return CCOLL_SUCCESS;

	void *tmp_data =
	    (void *)realloc(vec->data, (toalloc * vec->element_size));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data	  = tmp_data;
	vec->capacity = toalloc;

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
int Vec_alloc(Vec *vec, const size_t idxs) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;

	void *tmp_data = (void *)realloc(
	    vec->data, (vec->capacity + idxs) * vec->element_size
	);
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data = tmp_data;
	vec->capacity += idxs;

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
// TODO: apply the idea for handling on_remove foo
int Vec_free(Vec *vec) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;

	size_t errors = 0;
	if (vec->on_remove) {
		for (size_t i = 0; i < vec->size; i++) {
			if (vec->on_remove(
				  vec->data + (i * vec->element_size), vec->element_size
			    ))
				errors++;
		}
	}

	free(vec->data);
	free(vec);

	if (errors > 0) return CCOLL_PASSED_FOO_FAIL_CONTINUED;
	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
// TODO: apply the idea for handling on_remove foo
int Vec_free_range(Vec *vec, size_t from_idx, size_t to_idx) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (from_idx > to_idx) return CCOLL_INVALID_ARGUMENT;
	if (to_idx > vec->size) return CCOLL_INVALID_ARGUMENT;

	size_t errors = 0;
	if (vec->on_remove) {
		for (size_t i = from_idx; i < to_idx; i++) {
			if (vec->on_remove(
				  vec->data + (i * vec->element_size), vec->element_size
			    ))
				errors++;
		}
	}

	memmove(
	    vec->data + (from_idx * vec->element_size),
	    vec->data + (to_idx * vec->element_size),
	    (vec->size - to_idx) * vec->element_size
	);
	vec->size -= (to_idx - from_idx);

	void *tmp_data = realloc(
	    vec->data, (vec->capacity - (to_idx - from_idx)) * vec->element_size
	);
	// TODO:FIX: think how to handle error here, and on_remove fn call's (they
	// shouldn't happen if operation failed) but you can't do them after
	// realloc() cause that data doesn't exist anymore and tmp copying that is
	// to expensive
	// TODO: make new error (something like mem free failed but rest of the
	// code worked correctly)
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;
	vec->data = tmp_data;
	vec->capacity -= (to_idx - from_idx);

	if (errors) return CCOLL_PASSED_FOO_FAIL_CONTINUED;
	return CCOLL_SUCCESS;
}

// TODO:FIX: make that actually free the size of one element
// TODO:TEST: Make test for that foo
// TODO: apply the idea for handling on_remove foo
int Vec_free_element(Vec *vec, size_t idx) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (idx >= vec->size) return CCOLL_INVALID_ARGUMENT;

	if (vec->on_remove) {
		size_t errors = 0;
		if (vec->on_remove(
			  vec->data + (idx * vec->element_size), vec->element_size
		    ))
			errors++;
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

// TODO:TEST: Make test for that foo
int Vec_shrink_to(Vec *vec, const size_t idxs) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (idxs < vec->size) return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;

	void *tmp_data = realloc(vec->data, idxs * vec->element_size);
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data	  = tmp_data;
	vec->capacity = idxs;

	return CCOLL_SUCCESS;
}
