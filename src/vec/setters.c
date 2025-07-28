#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../ccoll-codes.h"
#include "../../colors.h"
#include "../../include/vec.h"

int Vec_set(Vec *vec, const size_t idx, const void *data) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (!data) return CCOLL_NULL_DATA;
	if (idx > vec->size) return CCOLL_INVALID_ELEMENT;
	if (idx > SIZE_MAX) return CCOLL_OVERFLOW;

	if (idx == vec->size) {
		if (Vec_alloc(vec, 1)) return CCOLL_OUT_OF_MEMORY;
		memcpy(Vec_get_unchecked(vec, idx), data, Vec_idx_to_bytes(vec, 1));
		vec->size++;
		return CCOLL_SUCCESS;
	} else {
		if (vec->on_remove) {
			switch (vec->on_remove(
			    Vec_get(vec, idx), idx, vec->element_size,
			    CCOLL_OPERATION_REPLACE
			)) {
			case CCOLL_CALLBACK_NOTHING: break;
			case CCOLL_CALLBACK_CANCEL: return CCOLL_CANCELED;
			case CCOLL_CALLBACK_DESTROY_VEC:
				Vec_free(vec);
				return CCOLL_DESTROYED;
				break;
			default: break;
			};
		}
	};

	memcpy(vec->data + (idx * vec->element_size), data, vec->element_size);

	return CCOLL_SUCCESS;
}

// IMPORTANT: completely refactor that foo it works (actually it does not work
// lol) but it is so messy I don't even want to look at it
int Vec_set_range(
    Vec *vec, const void *data, size_t start_idx, const size_t quantity
) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (!data) return CCOLL_NULL_DATA;
	if (start_idx > vec->size) return CCOLL_INVALID_ELEMENT;
	if (start_idx + quantity > SIZE_MAX) return CCOLL_OVERFLOW;

	if (quantity == 0) return CCOLL_SUCCESS;

	size_t end_idx = start_idx + quantity;

	if (end_idx > vec->capacity) {
		if (Vec_reserve_entire(vec, end_idx)) return CCOLL_OUT_OF_MEMORY;
	}

	size_t omitted = 0;
	bool canceled  = false;
	if (vec->on_remove) {
		for (size_t i = start_idx; i < start_idx + quantity; i++) {
			switch (vec->on_remove(
			    Vec_get_unchecked(vec, i), i, vec->element_size,
			    CCOLL_OPERATION_REPLACE
			)) {
			case CCOLL_CALLBACK_CANCEL:
				canceled = true;
				if (i >= vec->size) omitted++;
				start_idx--;
				continue;
			case CCOLL_CALLBACK_DESTROY_VEC:
				Vec_free(vec);
				return CCOLL_DESTROYED;
			case CCOLL_CALLBACK_NOTHING:
			default:
				memcpy(
				    Vec_get_unchecked(vec, i),
				    // TODO: think how to make below line more nice for
				    // the eye
				    data + ((i - start_idx) * vec->element_size),
				    Vec_idx_to_bytes(vec, 1)
				);
				break;
			}
		}
		if (end_idx > vec->size) {
			vec->size = end_idx - omitted;
		}
	} else {
		memcpy(
		    Vec_get_unchecked(vec, start_idx), data,
		    Vec_idx_to_bytes(vec, quantity)
		);
		if (end_idx >= vec->size) {
			vec->size = end_idx;
		}
	}

	if (canceled) return CCOLL_SUCCESS_WITH_CANCELED;
	return CCOLL_SUCCESS;
}

int Vec_push(Vec *vec, const void *data) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (!data) return CCOLL_NULL_DATA;
	if (vec->size + 1 > SIZE_MAX) return CCOLL_OVERFLOW;

	if (vec->capacity == vec->size) {
		if (Vec_alloc(vec, vec->capacity)) return CCOLL_OUT_OF_MEMORY;
	}

	memcpy(
	    vec->data + (vec->size * vec->element_size), data, vec->element_size
	);
	vec->size++;
	return CCOLL_SUCCESS;
}

int Vec_push_front(Vec *vec, const void *data) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (!data) return CCOLL_NULL_DATA;
	if (vec->size + 1 > SIZE_MAX) return CCOLL_OVERFLOW;

	if (vec->capacity <= vec->size) {
		if (Vec_alloc(vec, vec->capacity) != CCOLL_SUCCESS)
			return CCOLL_OUT_OF_MEMORY;
	}

	memmove(
	    vec->data + vec->element_size, vec->data,
	    vec->size * vec->element_size
	);
	memcpy(vec->data, data, vec->element_size);
	vec->size++;

	return CCOLL_SUCCESS;
}

int Vec_push_range(Vec *vec, const void *data, size_t quantity) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (!data) return CCOLL_NULL_DATA;
	// TODO: check if you need both checks
	if (quantity > SIZE_MAX) return CCOLL_OVERFLOW;
	if (vec->size + quantity > SIZE_MAX) return CCOLL_OVERFLOW;

	if (quantity == 0) return CCOLL_SUCCESS;

	if (vec->size + quantity > vec->capacity) {
		if (Vec_reserve(vec, quantity)) return CCOLL_OUT_OF_MEMORY;
	}

	memcpy(
	    Vec_get_unchecked(vec, vec->size), data,
	    Vec_idx_to_bytes(vec, quantity)
	);

	vec->size += quantity;

	return CCOLL_SUCCESS;
}

int Vec_push_front_range(Vec *vec, const void *data, size_t quantity) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (!data) return CCOLL_NULL_DATA;
	// TODO: check if you need both checks
	if (quantity > SIZE_MAX) return CCOLL_OVERFLOW;
	if (vec->size + quantity > SIZE_MAX) return CCOLL_OVERFLOW;

	if (quantity == 0) return CCOLL_SUCCESS;

	if (vec->size + quantity > vec->capacity) {
		if (Vec_reserve(vec, quantity)) return CCOLL_OUT_OF_MEMORY;
	}

	memmove(
	    Vec_get_unchecked(vec, quantity), vec->data,
	    Vec_idx_to_bytes(vec, vec->size)
	);
	memcpy(vec->data, data, Vec_idx_to_bytes(vec, quantity));

	vec->size += quantity;

	return CCOLL_SUCCESS;
}

int Vec_insert(Vec *vec, const size_t idx, const void *data) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (!data) return CCOLL_NULL_DATA;
	if (idx > vec->size) return CCOLL_INVALID_ELEMENT;
	if (idx > SIZE_MAX) return CCOLL_OVERFLOW;

	if (vec->size >= vec->capacity) {
		if (Vec_alloc(vec, vec->capacity)) return CCOLL_OUT_OF_MEMORY;
	}

	memmove(
	    Vec_get_unchecked(vec, idx + 1), Vec_get_unchecked(vec, idx),
	    Vec_idx_to_bytes(vec, vec->size - idx)
	);
	memcpy(Vec_get_unchecked(vec, idx), data, Vec_idx_to_bytes(vec, 1));
	vec->size++;

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
int Vec_insert_range(
    Vec *vec, const void *data, const size_t start_idx, const size_t quantity
) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (!data) return CCOLL_NULL_DATA;
	if (start_idx > vec->size) return CCOLL_INVALID_ELEMENT;
	if (start_idx + quantity > SIZE_MAX) return CCOLL_OVERFLOW;

	if (quantity == 0) return CCOLL_SUCCESS;

	if (vec->size + quantity > vec->capacity) {
		if (Vec_reserve(vec, quantity)) return CCOLL_OUT_OF_MEMORY;
	}

	memmove(
	    vec->data + ((start_idx + quantity) * vec->element_size),
	    vec->data + (start_idx * vec->element_size),
	    (vec->size - start_idx) * vec->element_size
	);

	memcpy(
	    vec->data + (start_idx * vec->element_size), data,
	    quantity * vec->element_size
	);

	vec->size += quantity;

	return CCOLL_SUCCESS;
}

int Vec_append(Vec *base, const Vec *vec) {
	if (!base || !vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (base->size + vec->size > SIZE_MAX) return CCOLL_OVERFLOW;
	if (base->element_size != vec->element_size)
		return CCOLL_ELEMENT_SIZE_MISMATCH;

	if ((base->capacity - base->size) < vec->size) {
		size_t toalloc = base->size + vec->size;
		if (Vec_reserve(base, toalloc)) return CCOLL_OUT_OF_MEMORY;
	}

	// TODO: check if that base operations and pointer calculations are safe
	memcpy(
	    base->data + (base->size * base->element_size), vec->data,
	    Vec_idx_to_bytes(vec, vec->size)
	);

	base->size += vec->size;

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
Vec *Vec_append_clone(const Vec *vec1, const Vec *vec2) {
	if (!vec1 || !vec2) return NULL;
	if (!vec1->data || !vec2->data) return NULL;
	if (vec1->size + vec2->size > SIZE_MAX) return NULL;
	if (vec1->element_size != vec2->element_size) return NULL;

	Vec *vec = Vec_init_with(vec1->element_size, vec1->size + vec2->size);
	if (!vec) return NULL;

	memcpy(vec->data, vec1->data, vec1->size * vec1->element_size);
	memcpy(
	    vec->data + (vec1->size * vec1->element_size), vec2->data,
	    vec2->size * vec2->element_size
	);

	if (vec1->on_remove)
		vec->on_remove = vec1->on_remove;
	else if (vec2->on_remove)
		vec->on_remove = vec2->on_remove;

	vec->size = vec1->size + vec2->size;

	return vec;
}

int Vec_fill(Vec *vec, const void *data) {
	if (!vec) return CCOLL_NULL;
	if (!vec->data) return CCOLL_NULL_INTERNAL_DATA;
	if (!data) return CCOLL_NULL_DATA;

	if (vec->on_remove) {
		for (size_t i = 0; i < vec->size; i++) {
			vec->on_remove(
			    Vec_get(vec, i), i, vec->element_size,
			    CCOLL_OPERATION_REPLACE_FORCED
			);
		}
	}

	for (size_t i = 0; i < vec->capacity; i++) {
		memcpy(Vec_get_unchecked(vec, i), data, Vec_idx_to_bytes(vec, 1));
	}

	vec->size = vec->capacity;

	return CCOLL_SUCCESS;
}
