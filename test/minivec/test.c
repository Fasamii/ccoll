#include "../../include/minivec.h"
#include <stdio.h>

int main(void) {

	MiniVec *vec = MiniVec_init(sizeof(char));
	for (size_t i = 0; i < 10000; i++) {
		MiniVec_free_safe(vec);
		vec = MiniVec_init(sizeof(char));
		if (vec->capacity != CCOLL_MINIVEC_MIN_CAPACITY) {
			CCOLL_MINIVEC_ERROR("garbage capacity after init\n");
			return 1;
		}
	}

	MiniVec_reserve_additional(vec, 333);
	MiniVec_alloc(vec, 7);

	MiniVec_change_capacity(vec, 24);

	void *tmp_data = vec->data;
	vec->data	   = NULL;

	MiniVec_change_capacity(vec, 23);

	MiniVec *vec2 = MiniVec_init(8931231923993402351, .capacity = 12);
	MiniVec_free_safe(vec2);

	vec2 = MiniVec_init(8931231923993402351, .capacity = 12);

	MiniVec_change_capacity(vec, 25);

	void *tmp_vec = vec;
	vec		  = NULL;

	MiniVec_alloc(vec2, 200);

	MiniVec_change_capacity(vec, 25);

	vec	    = tmp_vec;
	vec->data = tmp_data;

	MiniVec_change_capacity(vec, 25);

	MiniVec_change_capacity(vec, 0);
	MiniVec_alloc(vec, 0);
	MiniVec_reserve_additional(vec, 0);
	MiniVec_change_capacity(vec, 0);

	MiniVec_free_safe(vec);

	return 0;
}
