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

	size_t errors = 0;
	for (size_t i = 0; i < vec->size; i++) {
		if (fn(vec->data + (i * vec->element_size), i, vec->element_size))
			errors++;
	}

	if (errors) return CCOLL_PASSED_FOO_FAIL_CONTINUED;
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
	if (vec->after_element) new->after_element = vec->after_element;

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
