#include "../../ccoll_errors.h"
#include "../../include/vec.h"
#include <stdbool.h>
#include <stdlib.h>

// TODO:TEST: Make test for that foo
int Vec_for_each(
    Vec *vec, int (*fn)(void *element, size_t idx, size_t element_size)
) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;
	if (!vec->data) return CCOLL_INVALID_ARGUMENT;
	if (!fn) return CCOLL_INVALID_ARGUMENT;

	// TODO:TEST: test that callback implementation
	for (size_t i = 0; i < vec->size; i++) {
		switch (
		    fn(vec->data + (i * vec->element_size), i, vec->element_size)
		) {
		case 0: break;
		case 1: Vec_remove(vec, i); break;
		case 2: i++; break;
		case 3:
			Vec_free(vec);
			return CCOLL_DESTROYED;
			break;
		}
	}

	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
Vec *Vec_filter(
    Vec *vec, bool (*fn)(void *element, size_t idx, size_t element_size)
) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (!fn) return NULL;

	Vec *new = Vec_init_with(
	    vec->element_size, vec->size > 16 ? vec->size / 2 : vec->size
	);
	if (!new) return NULL;
	if (vec->on_remove) new->on_remove= vec->on_remove;

	for (size_t i = 0; i < vec->size; i++) {
		if (fn(vec->data + (i * vec->element_size), i, vec->element_size)) {
			if (Vec_push(new, vec->data + (i * vec->element_size))) {
				Vec_free(new);
				return NULL;
			};
		}
	}

	return new;
}
