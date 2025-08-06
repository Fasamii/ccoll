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

void *Vec_pop_back(Vec *vec) {
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

	if (vec->on_change) {
		switch (vec->on_change(
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

int Vec_remove_range(Vec *vec, const size_t from_idx, const size_t to_idx) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (vec->size < to_idx) return CCOLL_INVALID_RANGE;
	if (from_idx > to_idx) return CCOLL_INVALID_ELEMENT;

	if (from_idx == to_idx) return CCOLL_SUCCESS;

	size_t range = (to_idx - from_idx);

	if (vec->on_change) {
		bool canceled = false;
		size_t write  = from_idx;

		for (size_t read = from_idx; read < to_idx; read++) {
			void *element = Vec_get_unchecked_ptr(vec, read);
			switch (vec->on_change(
			    element, read, vec->element_size, CCOLL_OPERATION_REMOVE
			)) {
			case CCOLL_CALLBACK_NOTHING: break;
			case CCOLL_CALLBACK_CANCEL:
				canceled = true;
				if (read != write) {
					memcpy(
					    Vec_get_unchecked_ptr(vec, write), element,
					    Vec_idx_to_bytes(vec, 1)
					);
				}
				write++;
				break;
			case CCOLL_CALLBACK_DESTROY_VEC:
				Vec_free(vec);
				return CCOLL_DESTROYED;
			default: break;
			}
		}

		size_t removed = to_idx - write;

		memmove(
		    Vec_get_unchecked_ptr(vec, write),
		    Vec_get_unchecked_ptr(vec, to_idx),
		    Vec_idx_to_bytes(vec, vec->size - to_idx)
		);

		vec->size -= removed;

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

	if (!vec->on_change) {
		vec->size = 0;
		return CCOLL_SUCCESS;
	}

	size_t original_size = vec->size;
	size_t keep_pos = 0; // Position to write elements that should be kept

	// Process elements in-place, keeping only canceled removals
	for (size_t i = 0; i < original_size; i++) {
		void *element_ptr = Vec_get_unchecked_ptr(vec, i);

		switch (vec->on_change(
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
