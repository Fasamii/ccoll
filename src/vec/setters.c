#include "../../ccoll_errors.h"
#include "../../colors.h"
#include "../../include/vec.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int Vec_set(Vec *vec, const size_t idx, const void *data) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;
	if (idx > vec->size) return CCOLL_INVALID_ARGUMENT;

	if (idx == vec->size) {
		if (Vec_alloc(vec, 1) != CCOLL_SUCCESS) return CCOLL_OUT_OF_MEMORY;
		memcpy(
		    vec->data + (idx * vec->element_size), data, vec->element_size
		);
		vec->size++;
		return CCOLL_SUCCESS;
	} else {
		if (vec->on_remove) {
			switch (vec->on_remove(
			    vec->data + (idx * vec->element_size), vec->element_size
			)) {
			case 0: break;
			// TODO: change that success into some half error codes like
			// Canceled
			case 1: return CCOLL_SUCCESS;
			case 2: return CCOLL_SUCCESS;
			case 3:
				Vec_free(vec);
				return CCOLL_DESTROYED;
				break;
			default: /* TODO: when you make logging put log here about not
					handled code */
				break;
			};
		}
	};

	memcpy(vec->data + (idx * vec->element_size), data, vec->element_size);

	return CCOLL_SUCCESS;
}

int Vec_push(Vec *vec, const void *data) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;

	if (vec->capacity == vec->size) {
		if (Vec_alloc(vec, vec->capacity) != CCOLL_SUCCESS)
			return CCOLL_OUT_OF_MEMORY;
	}

	memcpy(
	    vec->data + (vec->size * vec->element_size), data, vec->element_size
	);
	vec->size++;
	return CCOLL_SUCCESS;
}

int Vec_push_front(Vec *vec, const void *data) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;

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

int Vec_insert(Vec *vec, const size_t idx, const void *data) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;
	if (idx > vec->size) return CCOLL_INVALID_ARGUMENT;

	if (vec->size >= vec->capacity) {
		if (Vec_alloc(vec, vec->capacity) != CCOLL_SUCCESS)
			return CCOLL_OUT_OF_MEMORY;
	}

	memmove(
	    vec->data + ((idx + 1) * vec->element_size),
	    vec->data + ((idx)*vec->element_size),
	    (vec->size - idx) * vec->element_size
	);
	memcpy(vec->data + (idx * vec->element_size), data, vec->element_size);
	vec->size++;

	return CCOLL_SUCCESS;
}

int Vec_append(Vec *base, const Vec *vec) {
	if (!base || !vec) return CCOLL_INVALID_ARGUMENT;
	if (!base->data || !vec->data) return CCOLL_INVALID_ARGUMENT;
	if (base->element_size != vec->element_size)
		return CCOLL_INVALID_ARGUMENT;

	if ((base->capacity - base->size) < vec->size) {
		size_t toalloc = base->size + vec->size;
		if (Vec_reserve(base, toalloc) != CCOLL_SUCCESS)
			return CCOLL_OUT_OF_MEMORY;
	}

	memcpy(
	    base->data + (base->size * base->element_size), vec->data,
	    vec->size * base->element_size
	);

	base->size += vec->size;

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
Vec *Vec_append_clone(const Vec *vec1, const Vec *vec2) {
	if (!vec1 || !vec2) return NULL;
	if (!vec1->data || !vec2->data) return NULL;
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

// IMPORTANT:TEST: that foo, at this point I don't know if it's even valid (I am
// to tired rn (literally hands are shaking lol)) i will probably check that
// tomorrow
int Vec_set_range(
    Vec *vec, const void *data, const size_t start_idx, const size_t quantity
) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;
	if (start_idx > vec->size) return CCOLL_INVALID_ARGUMENT;
	// TODO: make overflow checks for other foo's
	if (quantity > SIZE_MAX - start_idx) return CCOLL_OVERFLOW;

	if (quantity == 0) return CCOLL_SUCCESS;

	size_t end_idx = start_idx + quantity;

	if (end_idx > vec->capacity) {
		if (Vec_reserve(vec, end_idx - vec->size))
			return CCOLL_OUT_OF_MEMORY;
	}

	if (vec->on_remove && start_idx < vec->size) {
		size_t omitted = 0;
		for (size_t i = 0; i < quantity; i++) {
			switch (vec->on_remove(
			    vec->data + ((i + quantity) * vec->element_size),
			    vec->element_size
			)) {
			case 0:
				memcpy(
				    vec->data + ((i + start_idx) * vec->element_size),
				    data + ((i)*vec->element_size), vec->element_size
				);
				break;
			case 1: omitted++; continue;
			case 2: Vec_free(vec); return CCOLL_DESTROYED;
			}
		}
		if (end_idx > vec->size) {
			vec->size = end_idx - omitted;
		}
	} else {
		memcpy(
		    vec->data + (start_idx * vec->element_size), data,
		    quantity * vec->element_size
		);
		if (end_idx > vec->size) {
			vec->size = end_idx;
		}
	}

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
int Vec_insert_range(
    Vec *vec, const void *data, const size_t start_idx, const size_t quantity
) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;
	if (start_idx > vec->size) return CCOLL_INVALID_ARGUMENT;

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

// TODO: read implementation of that foo and apply to rest of range foo's
int Vec_push_range(Vec *vec, const void *data, size_t quantity) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;

	if (quantity == 0) return CCOLL_SUCCESS;

	if (vec->size + quantity > vec->capacity) {
		if (Vec_reserve(vec, quantity)) return CCOLL_OUT_OF_MEMORY;
	}

	memcpy(
	    vec->data + (vec->size * vec->element_size), data,
	    quantity * vec->element_size
	);

	vec->size += quantity;

	return CCOLL_SUCCESS;
}

int Vec_push_front_range(Vec *vec, const void *data, size_t quantity) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;

	if (quantity == 0) return CCOLL_SUCCESS;

	if (vec->size + quantity > vec->capacity) {
		if (Vec_reserve(vec, quantity)) return CCOLL_OUT_OF_MEMORY;
	}

	memmove(
	    vec->data + (quantity * vec->element_size), vec->data,
	    vec->size * vec->element_size
	);
	memcpy(vec->data, data, quantity * vec->element_size);

	vec->size += quantity;

	return CCOLL_SUCCESS;
}

int Vec_fill(Vec *vec, const void *data) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;

	if (vec->on_remove) {
		int errors = 0;
		for (size_t i = 0; i < vec->size; i++) {
			if (vec->on_remove(
				  vec->data + (i * vec->element_size), vec->element_size
			    ))
				errors++;
		}
	}

	for (size_t i = 0; i < vec->capacity; i++) {
		memcpy(
		    vec->data + (i * vec->element_size), data, vec->element_size
		);
	}

	vec->size = vec->capacity;

	return CCOLL_SUCCESS;
}
