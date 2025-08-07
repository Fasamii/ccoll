#include "../../include/minivec.h"

int main(void) {

	MiniVec *vec = MiniVec_init(sizeof(char));
	MiniVec_reserve_additional(vec, 12);
	MiniVec_alloc(vec, 12);

	MiniVec_change_capacity(vec, 24);

	void *tmp_data = vec->data;
	vec->data = NULL;

	MiniVec_change_capacity(vec, 23);

	MiniVec *vec2 = MiniVec_init(8931231923993402351);

	MiniVec_change_capacity(vec, 25);

	void *tmp_vec = vec;
	vec = NULL;

	MiniVec_alloc(vec2, 200);

	MiniVec_change_capacity(vec, 25);

	vec = tmp_vec;
	vec->data = tmp_data;

	MiniVec_change_capacity(vec, 25);

	MiniVec_change_capacity(vec, 0);
	MiniVec_alloc(vec, 0);
	MiniVec_reserve_additional(vec, 0);
	MiniVec_change_capacity(vec, 0);

	return 0;
}
