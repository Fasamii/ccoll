#include "../../colors.h"
#include "../../include/vec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int _Vec_realloc_checked(Vec *vec, size_t capacity_target) {
	if (!vec) return CCOLL_INVALID_ARGUMENT;

	if (!vec->data) {
		vec->data = malloc(capacity_target * vec->element_size);
		if (!vec->data) return CCOLL_OUT_OF_MEMORY;
		vec->capacity = capacity_target;
	} else {
		if (vec->capacity < capacity_target) {
			void *tmp_data = realloc(vec->data, capacity_target * vec->element_size);
			if (!tmp_data) return CCOLL_OUT_OF_MEMORY;
			vec->data = tmp_data;
			vec->capacity = capacity_target;
		}
	}

	return CCOLL_SUCCESS;
}
