#include "../../include/vec.h"
#include "../../ccoll_errors.h"
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

	memmove(
	    vec->data + (idx * vec->element_size),
	    vec->data + ((idx + 1) * vec->element_size),
	    (vec->size - idx - 1) * vec->element_size
	);

	vec->size--;

	return CCOLL_SUCCESS;
}
