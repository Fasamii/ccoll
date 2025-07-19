#include "../../colors.h"
#include "../../include/vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Vec *Vec_init(size_t size_of_data) {

	Vec *vec = (Vec *)malloc(sizeof(Vec));
	if (!vec) return NULL;

	vec->size		= 0;
	vec->element_size = size_of_data;
	vec->capacity	= VEC_MIN_CAPACITY;
	vec->destructor	= NULL;

	vec->data = (void *)malloc(vec->capacity * vec->element_size);
	if (!vec->data) {
		free(vec);
		return NULL;
	}

	return vec;
}

Vec *Vec_init_with(size_t size_of_data, size_t min_capacity) {
	Vec *vec = (Vec *)malloc(sizeof(Vec));
	if (!vec) return NULL;

	vec->size		= 0;
	vec->element_size = size_of_data;
	vec->capacity	= min_capacity;
	vec->destructor	= NULL;

	vec->data = (void *)malloc(vec->capacity * vec->element_size);
	if (!vec->data) {
		free(vec);
		return NULL;
	}

	return vec;
}

int Vec_set_destructor(Vec *vec, int (*destructor)(void *)) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	vec->destructor = destructor;
	return CCOLL_SUCCESS;
}

int Vec_set(Vec *vec, size_t idx, void *data) {
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
	};

	memcpy(vec->data + (idx * vec->element_size), data, vec->element_size);

	return CCOLL_SUCCESS;
}

int Vec_push(Vec *vec, void *data) {
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

int Vec_push_back(Vec *vec, void *data) {
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

int Vec_insert(Vec *vec, size_t idx, void *data) {
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

int Vec_append(Vec *base, Vec *vec) {
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

Vec *Vec_append_clone(Vec *vec1, Vec *vec2) {
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

	if (vec1->destructor)
		vec->destructor = vec1->destructor;
	else if (vec2->destructor)
		vec->destructor = vec2->destructor;

	vec->size = vec1->size + vec2->size;

	return vec;
}

int Vec_set_range(Vec *vec, void **data, size_t start_idx, size_t quantity) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;
	if (!*data) return CCOLL_INVALID_ARGUMENT;
	if (start_idx > vec->size) return CCOLL_INVALID_ARGUMENT;

	if (quantity == 0) return CCOLL_SUCCESS;

	size_t data_replaced = vec->size - start_idx;

	if (start_idx + quantity > vec->capacity) {
		if (Vec_reserve(vec, quantity - data_replaced))
			return CCOLL_OUT_OF_MEMORY;
	}

	memmove(
	    vec->data + (start_idx * vec->element_size), *data,
	    quantity * vec->element_size
	);

	vec->size += quantity - data_replaced;

	return CCOLL_SUCCESS;
}

int Vec_insert_range(Vec *vec, void **data, size_t start_idx, size_t quantity) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;
	if (!*data) return CCOLL_INVALID_ARGUMENT;
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

	memmove(
	    vec->data + (start_idx * vec->element_size), *data,
	    quantity * vec->element_size
	);

	vec->size += quantity;

	return CCOLL_SUCCESS;
}
