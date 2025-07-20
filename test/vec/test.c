#include "../../ccoll_errors.h"
#include "../../colors.h"
#include "../../include/vec.h"

#include "../util.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Covers: Vec_init(), Vec_init_with(), Vec_free()
void TEST_inti_and_free(size_t sizeof_data) {
	Vec *vec = Vec_init(sizeof_data);

	assert_eq(
	    vec != NULL && vec->size == 0 && vec->capacity == VEC_MIN_CAPACITY &&
		  vec->element_size == sizeof_data,
	    "Vec_init()"
	);

	Vec_free(vec);

	vec = Vec_init_with(sizeof_data, 89);
	assert_eq(vec != NULL, "Vec_init_with()");
	log("element_size: %ld", vec->element_size);

	Vec_free(vec);
}

// Covers: Vec_push(), Vec_push_front(), Vec_pop(), Vec_pop_front()
void TEST_push_pop(int count) {
	Vec *vec = Vec_init(sizeof(char));

	char ch;
	for (int i = 0; i < count; i++) {
		ch = random_alphanumeric();
		Vec_push(vec, &ch);
		char res = *(char *)Vec_pop(vec);
		assert_eq(
		    res == ch,
		    "Vec_pop() Should return " BLU "%c" NOCOL " returned " BLU
		    "%c" NOCOL,
		    ch, res
		);
	}

	for (int i = 0; i < count; i++) {
		ch = random_alphanumeric();
		Vec_push_front(vec, &ch);
		char res = *(char *)Vec_pop_front(vec);
		assert_eq(
		    res == ch,
		    "Vec_pop_front() Should return " BLU "%c" NOCOL " returned " BLU
		    "%c" NOCOL,
		    ch, res
		);
	}

	Vec_free(vec);
	return;
}

// Covers: Vec_push(), Vec_pop(), Vec_push_front(), Vec_pop_front()
void TEST_push_pop_edge() {
	int res;
	Vec *vec = Vec_init(sizeof(char));

	res = Vec_push(vec, NULL);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_push() should fail if invalid arguments are provided"
	);

	char ch = 'a';
	res	  = Vec_push(NULL, &ch);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_push() should fail if invalid arguments are provided"
	);

	res = Vec_push(NULL, NULL);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_push() should fail if invalid arguments are provided"
	);

	res = Vec_push_front(vec, NULL);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_push_front() should fail if invalid arguments are provided"
	);

	ch  = 'b';
	res = Vec_push_front(NULL, &ch);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_push_front() should fail if invalid arguments are provided"
	);

	res = Vec_push_front(NULL, NULL);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_push_front() should fail if invalid arguments are provided"
	);

	void *data = Vec_pop(vec);
	assert_eq(
	    data == NULL,
	    "Vec_pop() should fail if invalid arguments are provided"
	);

	data = Vec_pop_front(vec);
	assert_eq(
	    data == NULL,
	    "Vec_pop_front() should fail if invalid arguments are provided"
	);

	Vec_free(vec);
}

// Covers: Vec_push_range(), Vec_push_front_range()
void TEST_push_range(int size) {
	Vec *vec = Vec_init_with(sizeof(char), size);

	char *str = (char *)malloc((size + 1) * sizeof(char));
	generate_random_string(str, size);

	Vec_push_range(vec, str, size);
	assert_eq(
	    memcmp(vec->data, str, size) == 0, "(vec:size:%ld):Vec_push_range()",
	    vec->size
	);

	Vec_push_front_range(vec, str, size);
	assert_eq(
	    memcmp(vec->data, str, size) == 0,
	    "(vec:size:%ld):Vec_push_front_range()", vec->size
	);

	free(str);
	Vec_free(vec);
	return;
}

// Covers: Vec_init(), Vec_init_with()
void TEST_edge_init_and_free() {
	int res;

	Vec *vec = Vec_init(0);
	assert_eq(
	    vec == NULL,
	    "if sizeof_data for Vec_init() is 0 it should return NULL"
	);
	res = Vec_free(vec);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_free() should return CCOLL_INVALID_ARGUMENT"
	);

	vec = Vec_init_with(sizeof(char), 0);
	assert_eq(
	    vec == NULL,
	    "if min_capacity for Vec_init_with() is 0 it should return NULL"
	);
	res = Vec_free(vec);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_free() should return CCOLL_INVALID_ARGUMENT"
	);

	vec = Vec_init_with(0, 12);
	assert_eq(
	    vec == NULL,
	    "if sizeof_data for Vec_init_with() is 0 it should return NULL"
	);
	res = Vec_free(vec);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_free() should return CCOLL_INVALID_ARGUMENT"
	);

	vec = Vec_init_with(0, 0);
	assert_eq(
	    vec == NULL, "if sizeof_data and min_capacity for Vec_init_with() is "
			     "0 it should return NULL"
	);
	res = Vec_free(vec);
	assert_eq(
	    res == CCOLL_INVALID_ARGUMENT,
	    "Vec_free() should return CCOLL_INVALID_ARGUMENT"
	);
}

// Covers: Vec_fill(), Vec_pop(), Vec_pop_front()
void TEST_fill(size_t size) {
	Vec *vec	  = Vec_init_with(sizeof(double), size);
	double number = 10.10101012;
	Vec_fill(vec, &number);

	double last = *(double *)Vec_pop(vec);
	assert_eq(last == number, "Vec_pop() & Vec_fill() with size:%ld", size);

	double *first = (double *)Vec_pop_front(vec);
	if (size == 1) {
		assert_eq(first == NULL, "If you pop'ed only existing element second pop should return NULL");
		return;
	}
	assert_eq(
	    *first == number, "Vec_pop_front() & Vec_fill() with size:%ld", size
	);
}

int main(void) {
	section("Init & Free");
	TEST_inti_and_free(1);
	TEST_inti_and_free(16);
	TEST_inti_and_free(512);
	TEST_inti_and_free(1024);
	TEST_inti_and_free(1024000);
	section("Edge Testing: Init & Free");
	TEST_edge_init_and_free();
	section("Push");
	TEST_push_pop(10);
	section("Edge Testing: Push");
	TEST_push_pop_edge();
	section("Push range");
	TEST_push_range(1);
	TEST_push_range(2);
	TEST_push_range(38);
	TEST_push_range(308);
	TEST_push_range(800008);
	TEST_push_range(10000008);
	section("Fill");
	TEST_fill(1);
	TEST_fill(2);
	TEST_fill(300);
	TEST_fill(12900);
	TEST_fill(90000023);

	printf("\n");
	return 0;
}
