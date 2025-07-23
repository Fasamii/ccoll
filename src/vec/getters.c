#include "../../ccoll_errors.h"
#include "../../include/vec.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// TODO:TEST: Make test for that foo
void *Vec_get(const Vec *vec, const size_t idx) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (idx >= vec->size) return NULL;

	return vec->data + (idx * vec->element_size);
}

void *Vec_pop(Vec *vec) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (vec->size < 1) return NULL;

	void *data = (void *)malloc(vec->element_size);
	memcpy(
	    data, vec->data + ((vec->size - 1) * vec->element_size),
	    vec->element_size
	);

	vec->size--;

	return data;
}

void *Vec_pop_front(Vec *vec) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (vec->size < 1) return NULL;

	void *data = (void *)malloc(vec->element_size);
	if (!data) return NULL;

	memcpy(data, vec->data, vec->element_size);
	memcpy(
	    vec->data, vec->data + vec->element_size,
	    vec->size * vec->element_size
	);

	vec->size--;

	return data;
}

// TODO:TEST: Make test for that foo
int Vec_remove(Vec *vec, size_t idx) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (vec->size < 1) return CCOLL_EMPTY;
	if (idx >= vec->size) return CCOLL_INVALID_ARGUMENT;

	if (vec->after_rm) {
		// TODO: make that error handling that you described in
		// include/vec.h file
		size_t errors = 0;
		if (vec->after_rm(
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

// TODO:IMPORTANT:TEST: that foo
int Vec_remove_range(Vec *vec, size_t from_idx, size_t to_idx) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (to_idx >= from_idx) return CCOLL_INVALID_ARGUMENT;
	// TODO: consider adding below check to other foo's
	// TODO: consider making some macro or check foo instead of writing the
	// same code in every foo
	if ((to_idx - from_idx) > vec->size) return CCOLL_INVALID_ARGUMENT;

	if (vec->after_rm) {
		// TODO: make that error handling that you described in
		// include/vec.h file
		size_t errors = 0;
		for (size_t i = from_idx; i < to_idx; i++) {
			if (vec->after_rm(
				  vec->data + (i * vec->element_size), vec->element_size
			    ))
				errors++;
		}
	}

	// TODO: consider memcpy instead, read memcpy TODO comment in vec/setters.c
	memmove(
	    vec->data + (from_idx * vec->element_size),
	    vec->data + (to_idx * vec->element_size),
	    (vec->size - to_idx * vec->element_size)
	);

	vec->size -= (to_idx - from_idx);

	return CCOLL_SUCCESS;
}
