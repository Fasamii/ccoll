#include "../../include/minivec.h"
#include <stddef.h>
#include <stdio.h>

int main(void) {

	MiniVec *vec = MiniVec_init(sizeof(char));
	for (size_t i = 0; i < 100; i++) {
		MiniVec_free_safe(vec);
		size_t cap = 9;
		vec	     = MiniVec_init(2, .capacity = cap, .alignment = 1U << i);
		if (vec && vec->capacity != cap) {
			CCOLL_MINIVEC_ERROR(
			    "garbage capacity (%zu) after init\n", vec->capacity
			);
			return 1;
		}
		if (!vec) {
			CCOLL_MINIVEC_WARN("vec is null");
		}
	}
	MiniVec_free_safe(vec);
	vec = MiniVec_init(sizeof(char), .alignment = 8, .capacity = 9);

	MiniVec_reserve_additional(vec, 333);
	MiniVec_alloc(vec, 7);

	MiniVec_change_capacity(vec, 24);

	void *tmp_data = vec->data;
	vec->data	   = NULL;

	MiniVec_change_capacity(vec, 23);

	MiniVec *vec2 =
	    MiniVec_init(8931231923993402351, .capacity = 3, .alignment = 64);
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
