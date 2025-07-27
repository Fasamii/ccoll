#include "../../include/vec.h"

#include <stddef.h>

static size_t called = 0;

// TODO: cover 0,1,2 return codes
int CALLBACK_Vec_free_range(
    void *data, size_t idx, size_t element_size, CCOLL_OPERATION operation
) {
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
