#include "../../ccoll-codes.h"
#include "../../include/vec.h"
// #include <stdatomic.h> // TODO: check what that is
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int Vec_reserve(Vec *vec, const size_t idxs) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_DATA;

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
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_DATA;

	void *tmp_data =
	    realloc(vec->data, Vec_idx_to_bytes(vec, vec->capacity + idxs));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data = tmp_data;
	vec->capacity += idxs;

	return CCOLL_SUCCESS;
}

int Vec_free(Vec *vec) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_DATA;

	if (vec->on_remove) {
		for (size_t i = 0; i < vec->size; i++) {
			vec->on_remove(
			    Vec_get(vec, i), i, vec->element_size,
			    CCOLL_OPERATION_REMOVE_FORCED
			);
		}
	}

	free(vec->data);
	free(vec);

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
// TODO: make realloc()

int Vec_free_range(Vec *vec, size_t from_idx, size_t to_idx) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_DATA;
	if (from_idx > to_idx) return CCOLL_INVALID_RANGE;
	if (to_idx >= vec->size) return CCOLL_INVALID_ELEMENT;

	if (from_idx == to_idx) return CCOLL_SUCCESS;

	size_t range = to_idx - from_idx;

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
			    vec->data + (idx * vec->element_size),
			    vec->data + ((idx + 1) * vec->element_size),
			    (vec->size - idx) * vec->element_size
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
		memmove(
		    vec->data + (from_idx * vec->element_size),
		    vec->data + (to_idx * vec->element_size),
		    (vec->size - to_idx) * vec->element_size
		);

		vec->size -= range;

		void *tmp_data =
		    realloc(vec->data, (vec->capacity - range) * vec->element_size);
		if (!tmp_data) return CCOLL_NO_REMOVED_MEMORY;
		vec->data	  = tmp_data;
		vec->capacity = (vec->size - range);

		return CCOLL_SUCCESS;
	}
}

// TODO:TEST: Make test for that foo
int Vec_free_element(Vec *vec, size_t idx) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_DATA;
	if (idx >= vec->size) return CCOLL_INVALID_ELEMENT;

	if (vec->on_remove) {
		switch (vec->on_remove(
		    Vec_get(vec, idx), idx, vec->element_size,
		    CCOLL_OPERATION_REMOVE
		)) {
		case 0: break;
		case 1: return CCOLL_CANCELED;
		case 3: Vec_free(vec); return CCOLL_DESTROYED;
		}
	}

	memmove(
	    vec->data + (idx * vec->element_size),
	    vec->data + ((idx + 1) * vec->element_size),
	    (vec->size - idx - 1) * vec->element_size
	);

	vec->size--;
	void *tmp_data = (void *)realloc(vec->data, vec->size);
	if (!tmp_data) return CCOLL_NO_REMOVED_MEMORY;
	vec->data = tmp_data;

	return CCOLL_SUCCESS;
}

int Vec_shrink(Vec *vec) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_DATA;

	void *tmp_data = realloc(vec->data, (vec->size * vec->element_size));
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;
	vec->data	  = tmp_data;
	vec->capacity = vec->size;

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
int Vec_shrink_to(Vec *vec, const size_t idxs) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_DATA;
	if (idxs < vec->size) return CCOLL_NOT_ENOUGH_MEMORY_REQUESTED;

	void *tmp_data = realloc(vec->data, idxs * vec->element_size);
	if (!tmp_data) return CCOLL_OUT_OF_MEMORY;

	vec->data	  = tmp_data;
	vec->capacity = idxs;

	return CCOLL_SUCCESS;
}
