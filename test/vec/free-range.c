#include <stddef.h>
#include <stdio.h>

#include "../../include/vec.h"

static size_t called = 0;

int CALLBACK_Vec_free_range(
    void *data, size_t idx, size_t element_size, CCOLL_OPERATION operation
) {
	printf(
	    "data %c at [%ld] w size: %ld done operation %d", *(char *)data, idx,
	    element_size, operation
	);
	called++;
	return 0;
}

void TEST_Vec_free_range() {
	Vec *vec = Vec_init(sizeof(char));
	Vec_alloc(vec, 12);
	char ch[] = "abc";
	Vec_fill(vec, &ch[2]);

	called = 0;

	Vec_set_on_remove_callback(vec, CALLBACK_Vec_free_range);
}
