#include "../../ccoll_errors.h"
#include "../../include/vec.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// TODO:TEST: Make test for that foo
// TODO: change docs for that foo to tell that vec should be uninitialized and
// NULL
int Vec_split(Vec *base, Vec **new_vec, const size_t idx) {
	if (!base) return CCOLL_INVALID_ARGUMENT;
	if (idx > base->size) return CCOLL_INVALID_ARGUMENT;
	if (!base->data) return CCOLL_INVALID_ARGUMENT;
	if (!new_vec) return CCOLL_INVALID_ARGUMENT;

	Vec *new;
	if (!*new_vec) {
		new = Vec_init_with(base->element_size, base->size - idx);
		if (!new) return CCOLL_OUT_OF_MEMORY;
	} else {
		new = *new_vec;
	}

	new->element_size = base->element_size;
	if (base->on_remove) new->on_remove = base->on_remove;

	memcpy(
	    new->data, base->data + (idx * base->element_size),
	    (base->size - idx) * base->element_size
	);

	new->size  = base->size - idx;
	base->size = idx;

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
// TODO: change docs for that foo to tell that vec should be uninitialized and
// NULL
int Vec_split_clone(
    const Vec *base, Vec **new_vec1, Vec **new_vec2, const size_t idx
) {
	if (!base) return CCOLL_INVALID_ARGUMENT;
	if (!base->data) return CCOLL_INVALID_ARGUMENT;
	if (idx > base->size) return CCOLL_INVALID_ARGUMENT;
	if (!new_vec1) return CCOLL_INVALID_ARGUMENT;
	if (!new_vec2) return CCOLL_INVALID_ARGUMENT;

	Vec *new1;
	if (!*new_vec1) {
		new1 = Vec_init_with(base->element_size, idx);
		if (!new1) return CCOLL_OUT_OF_MEMORY;
	} else
		new1 = *new_vec1;

	Vec *new2;
	if (!*new_vec2) {
		new2 = Vec_init_with(base->element_size, idx);
		if (!new2) {
			Vec_free(new1);
			return CCOLL_OUT_OF_MEMORY;
		};
	} else
		new2 = *new_vec2;

	new1->element_size = base->element_size;
	new2->element_size = base->element_size;
	if (base->on_remove) {
		new1->on_remove = base->on_remove;
		new2->on_remove = base->on_remove;
	}

	memcpy(new1->data, base->data, idx * base->element_size);
	memcpy(
	    new2->data, base->data + (idx * base->element_size),
	    (base->size - idx) * base->element_size
	);

	new1->size = idx;
	new2->size = base->size - idx;

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
Vec *Vec_slice(const Vec *vec, size_t from_idx, size_t to_idx) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (from_idx > to_idx) return NULL;
	if (to_idx >= vec->size) return NULL;

	// TODO: consider removing and check if it is safe to cast size_t to int
	if ((int)from_idx < 0) from_idx = vec->size + from_idx;
	if ((int)to_idx < 0) to_idx = vec->size + to_idx;

	size_t slice_size = to_idx - from_idx;

	if (slice_size == 0) {
		Vec *new = Vec_init(vec->element_size);
		if (!new) return NULL;
		if (vec->on_remove) new->on_remove = vec->on_remove;
		return new;
	}

	Vec *new = Vec_init_with(vec->element_size, slice_size);
	if (!new) return NULL;
	if (vec->on_remove) new->on_remove = vec->on_remove;

	memmove(
	    new->data, vec->data + (from_idx * vec->element_size),
	    slice_size * vec->element_size
	);

	new->size = slice_size;

	return new;
}

// TODO:TEST: Make test for that foo
int Vec_swap(Vec *vec, size_t idx1, size_t idx2) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (idx1 >= vec->size) return CCOLL_INVALID_ARGUMENT;
	if (idx2 >= vec->size) return CCOLL_INVALID_ARGUMENT;

	if (idx1 == idx2) return CCOLL_SUCCESS;

	uint8_t *elem1 = (uint8_t *)vec->data + idx1 * vec->element_size;
	uint8_t *elem2 = (uint8_t *)vec->data + idx2 * vec->element_size;

	if (vec->element_size <= 8) {

		// swap byte-by-byte
		for (size_t i = 0; i < vec->element_size; i++) {
			elem1[i] ^= elem2[i];
			elem2[i] ^= elem1[i];
			elem1[i] ^= elem2[i];
		}

	} else if (vec->element_size <= 64) {

		// swap in 8-byte chunks
		size_t chunks	= vec->element_size / sizeof(uint64_t);
		uint64_t *ptr1_64 = (uint64_t *)elem1;
		uint64_t *ptr2_64 = (uint64_t *)elem2;

		for (size_t i = 0; i < chunks; i++) {
			ptr1_64[i] ^= ptr2_64[i];
			ptr2_64[i] ^= ptr1_64[i];
			ptr1_64[i] ^= ptr2_64[i];
		}

		// Handle remaining bytes
		size_t remaining = vec->element_size % sizeof(uint64_t);
		uint8_t *rem1    = elem1 + chunks * sizeof(uint64_t);
		uint8_t *rem2    = elem2 + chunks * sizeof(uint64_t);

		for (size_t i = 0; i < remaining; i++) {
			rem1[i] ^= rem2[i];
			rem2[i] ^= rem1[i];
			rem1[i] ^= rem2[i];
		}

	} else {

		// use temp buffer to store exchanging data
		const size_t BUFFER_SIZE = 256;

		uint8_t temp_buffer[BUFFER_SIZE];
		size_t bytes_left = vec->element_size;
		size_t offset	= 0;

		while (bytes_left > 0) {
			size_t chunk_size =
			    (bytes_left < BUFFER_SIZE) ? bytes_left : BUFFER_SIZE;

			// Copy elem1 chunk to temp buffer
			for (size_t i = 0; i < chunk_size; i++) {
				temp_buffer[i] = elem1[offset + i];
			}

			// Copy elem2 chunk to elem1
			for (size_t i = 0; i < chunk_size; i++) {
				elem1[offset + i] = elem2[offset + i];
			}

			// Copy temp buffer to elem2
			for (size_t i = 0; i < chunk_size; i++) {
				elem2[offset + i] = temp_buffer[i];
			}

			offset += chunk_size;
			bytes_left -= chunk_size;
		}
	}

	return CCOLL_SUCCESS;
}
