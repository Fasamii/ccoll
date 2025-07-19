#include "../../include/vec.h"
#include <stdint.h>
#include <string.h>

// temp
#include <stdio.h>

int Vec_split(Vec *base, Vec *new_vec, size_t idx) {
	if (!base || !new_vec) return CCOLL_INVALID_ARGUMENT;
	if (idx > base->size) return CCOLL_INVALID_ARGUMENT;
	if (!base->data) return CCOLL_INVALID_ARGUMENT;

	new_vec->element_size = base->element_size;

	if (_Vec_realloc_checked(new_vec, base->size - idx))
		return CCOLL_OUT_OF_MEMORY;

	if (base->after_element) {
		new_vec->after_element= base->after_element;
	}

	memcpy(
	    new_vec->data, base->data + (idx * base->element_size),
	    (base->size - idx) * base->element_size
	);

	new_vec->size = base->size - idx; // NOTE: depends on base->size
	base->size = idx; // NOTE: new_vec size is dependent on that so it have to
				// be set second

	return CCOLL_SUCCESS;
}

int Vec_split_clone(Vec *base, Vec *new_vec1, Vec *new_vec2, size_t idx) {
	if (!base || !new_vec1 || !new_vec2) return CCOLL_INVALID_ARGUMENT;
	if (idx > base->size) return CCOLL_INVALID_ARGUMENT;
	if (!base->data) return CCOLL_INVALID_ARGUMENT;

	new_vec1->element_size = base->element_size;
	new_vec2->element_size = base->element_size;

	if (_Vec_realloc_checked(new_vec1, idx)) return CCOLL_OUT_OF_MEMORY;
	if (_Vec_realloc_checked(new_vec2, base->size - idx))
		return CCOLL_OUT_OF_MEMORY;

	if (base->after_element) {
		new_vec1->after_element= base->after_element;
		new_vec2->after_element= base->after_element;
	}

	memcpy(new_vec1->data, base->data, idx * base->element_size);
	memcpy(
	    new_vec2->data, base->data + (idx * base->element_size),
	    (base->size - idx) * base->element_size
	);

	new_vec1->size = idx;
	new_vec2->size = base->size - idx;

	return CCOLL_SUCCESS;
}

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
