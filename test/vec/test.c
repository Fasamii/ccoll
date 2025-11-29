#include "../../include/vec.h"
#include <stddef.h>
#include <stdio.h>

int main(void) {

	Vec *vec = Vec_init(sizeof(char));
	for (size_t i = 0; i < 10; i++) {
		Vec_free_safe(vec);
		size_t cap = 9;
		vec	     = Vec_init(2, .capacity = cap, .alignment = 1U << i);
		if (vec && vec->capacity != cap) {
			CCOLL_ERROR(
			    "garbage capacity (%zu) after init\n", vec->capacity
			);
			return 1;
		}
		if (!vec) {
			CCOLL_WARN("vec is null");
		}
	}
	Vec_free_safe(vec);
	vec = Vec_init(sizeof(char), .alignment = 8, .capacity = 9);

	Vec_reserve_additional(vec, 333);
	Vec_alloc(vec, 7);

	Vec_change_capacity(vec, 24);

	void *tmp_data = vec->data;
	vec->data	   = NULL;

	Vec_change_capacity(vec, 23, .alignment = 64);

	Vec *vec2 =
	    Vec_init(8931231923993402351, .capacity = 3, .alignment = 64);
	Vec_free_safe(vec2);

	vec2 = Vec_init(8931231923993402351, .capacity = 12);

	Vec_change_capacity(vec, 25);

	void *tmp_vec = vec;
	vec		  = NULL;

	Vec_alloc(
	    vec2, 200, .alignment = 64,
	    .growth_strategy = CCOLL_GROWTH_STRATEGY_GOLDEN
	);

	Vec_change_capacity(vec, 25, .alignment = 64);

	vec	    = tmp_vec;
	vec->data = tmp_data;

	Vec_change_capacity(vec, 25, .alignment = 64);

	Vec_change_capacity(vec, 0, .alignment = 64);
	Vec_alloc(
	    vec, 0, .alignment = 64,
	    .growth_strategy = CCOLL_GROWTH_STRATEGY_GOLDEN
	);
	Vec_reserve_additional(
	    vec, 0, .alignment = 64,
	    .growth_strategy = CCOLL_GROWTH_STRATEGY_GOLDEN
	);
	Vec_change_capacity(vec, 0, .alignment = 64);

	Vec_free_safe(vec);

	return 0;
}
