#include "../../ccoll_errors.h"
#include "../../include/vec.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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
int Vec_remove(Vec *vec, const size_t idx) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_DATA;
	if (vec->size == 0) return CCOLL_EMPTY;
	if (idx >= vec->size) return CCOLL_INVALID_ELEMENT;

	if (vec->on_remove) {
		switch (vec->on_remove(
		    Vec_get(vec, idx), idx, vec->element_size,
		    CCOLL_OPERATION_REMOVE
		)) {
		case 0: break;
		case 1:
			return CCOLL_CANCELED;
		case 2: Vec_free(vec); return CCOLL_DESTROYED;
		}
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
int Vec_remove_range(Vec *vec, const size_t from_idx, const size_t to_idx) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_DATA;
	if (vec->size == 0) return CCOLL_EMPTY;
	if (to_idx >= from_idx) return CCOLL_INVALID_ELEMENT;
	if (to_idx > vec->size) return CCOLL_INVALID_RANGE;

	size_t range = (to_idx - from_idx);

	if (vec->on_remove) {
		Vec *omitted = Vec_init_with(sizeof(size_t), range);
		for (size_t i = from_idx; i < to_idx; i++) {
			switch (vec->on_remove(
			    Vec_get(vec, i), i, vec->element_size,
			    CCOLL_OPERATION_REMOVE
			)) {
			case 0: break;
			case 1: Vec_push(omitted, &i); break;
			case 2: Vec_free(vec); return CCOLL_DESTROYED;
			default: break;
			}
		}

		size_t removed = range - omitted->size;

		while (omitted->size > 0) {
			size_t idx = *(size_t *)Vec_pop_front(omitted);
			memmove(
			    Vec_get(vec, idx), Vec_get(vec, idx + 1),
			    Vec_idx_to_bytes(vec, vec->size - idx)
			);
		}

		vec->size -= removed;

		void *tmp_data = realloc(
		    vec->data, Vec_idx_to_bytes(vec, vec->capacity - removed)
		);
		if (!tmp_data) return CCOLL_NO_REMOVED_MEMORY;
		vec->capacity -= removed;
		vec->data = tmp_data;

		return CCOLL_SUCCESS;
	} else {
		// TODO: consider better use of variables in memmove like range
		// etc...
		memmove(
		    Vec_get(vec, from_idx), Vec_get(vec, to_idx),
		    Vec_idx_to_bytes(vec, vec->size - to_idx)
		);

		vec->size -= range;
	}

	return CCOLL_SUCCESS;
}
