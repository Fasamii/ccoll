#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../ccoll-codes.h"
#include "../../include/vec.h"

void *Vec_get_clone_unchecked(const Vec *vec, const size_t idx) {
	void *data = malloc(vec->element_size);
	if (!data) return NULL;

	memcpy(data, Vec_get_unchecked_ptr(vec, idx), Vec_idx_to_bytes(vec, 1));

	return data;
}

void *Vec_get_clone(const Vec *vec, const size_t idx) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (idx >= vec->size) return NULL;

	return Vec_get_clone_unchecked(vec, idx);
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
int Vec_remove(Vec *vec, const size_t idx) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (vec->size == 0) return CCOLL_EMPTY;
	if (idx >= vec->size) return CCOLL_INVALID_ELEMENT;

	if (vec->on_remove) {
		switch (vec->on_remove(
		    Vec_get_unchecked_ptr(vec, idx), idx, vec->element_size,
		    CCOLL_OPERATION_REMOVE
		)) {
		case CCOLL_CALLBACK_NOTHING: break;
		case CCOLL_CALLBACK_CANCEL: return CCOLL_CANCELED;
		case CCOLL_CALLBACK_DESTROY_VEC:
			Vec_free(vec);
			return CCOLL_DESTROYED;
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

// TODO:TEST: that foo
int Vec_remove_range(Vec *vec, const size_t from_idx, const size_t to_idx) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (to_idx > vec->size) return CCOLL_INVALID_RANGE;
	if (to_idx >= from_idx) return CCOLL_INVALID_ELEMENT;

	size_t range = (to_idx - from_idx);

	if (vec->on_remove) {
		bool canceled = false;
		Vec *omitted  = Vec_init_with(sizeof(size_t), range);
		for (size_t i = from_idx; i < to_idx; i++) {
			switch (vec->on_remove(
			    Vec_get_unchecked_ptr(vec, i), i, vec->element_size,
			    CCOLL_OPERATION_REMOVE
			)) {
			case CCOLL_CALLBACK_NOTHING: break;
			case CCOLL_CALLBACK_CANCEL:
				canceled = true;
				Vec_push(omitted, &i);
				break;
			case CCOLL_CALLBACK_DESTROY_VEC:
				Vec_free(vec);
				return CCOLL_DESTROYED;
			default: break;
			}
		}

		size_t removed = range - omitted->size;

		while (omitted->size > 0) {
			size_t idx = *(size_t *)Vec_pop_front(omitted);
			memmove(
			    Vec_get_unchecked_ptr(vec, idx),
			    Vec_get_unchecked_ptr(vec, idx + 1),
			    Vec_idx_to_bytes(vec, vec->size - idx)
			);
		}

		vec->size -= removed;

		void *tmp_data = realloc(
		    vec->data, Vec_idx_to_bytes(vec, vec->capacity - removed)
		);
		if (!tmp_data)
			return canceled
				     ? CCOLL_SUCCESS_WITH_NO_REMOVED_MEMORY
				     : CCOLL_SUCCESS_WITH_NO_REMOVED_MEMORY_WITH_CANCELED;
		vec->capacity -= removed;
		vec->data = tmp_data;

		return canceled ? CCOLL_SUCCESS_WITH_CANCELED : CCOLL_SUCCESS;
	} else {
		memmove(
		    Vec_get_unchecked_ptr(vec, from_idx),
		    Vec_get_unchecked_ptr(vec, to_idx),
		    Vec_idx_to_bytes(vec, vec->size - to_idx)
		);

		vec->size -= range;
	}

	return CCOLL_SUCCESS;
}

// TODO:TEST: that foo
int Vec_remove_all(Vec *vec) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;

	if (vec->size == 0) return CCOLL_SUCCESS;

	if (!vec->on_remove) {
		vec->size = 0;
		return CCOLL_SUCCESS;
	}

	size_t original_size = vec->size;
	size_t keep_pos = 0; // Position to write elements that should be kept

	// Process elements in-place, keeping only canceled removals
	for (size_t i = 0; i < original_size; i++) {
		void *element_ptr = Vec_get_unchecked_ptr(vec, i);

		switch (vec->on_remove(
		    element_ptr, i, vec->element_size, CCOLL_OPERATION_REMOVE
		)) {
		case CCOLL_CALLBACK_NOTHING: break;
		case CCOLL_CALLBACK_CANCEL:
			if (keep_pos != i) {
				memmove(
				    (char *)vec->data + (keep_pos * vec->element_size),
				    element_ptr, vec->element_size
				);
			}
			keep_pos++;
			break;
		case CCOLL_CALLBACK_DESTROY_VEC:
			Vec_free(vec);
			return CCOLL_DESTROYED;
		}
	}

	vec->size = keep_pos;
	return CCOLL_SUCCESS;
}
