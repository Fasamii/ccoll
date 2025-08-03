#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../../include/vec.h"

#include "../../ccoll-codes.h"
#include "../../colors.h"
#include "../util.h"

#include "free-range.c"
#include "remove-range.c"

// Covers: Vec_init(), Vec_init_with(), Vec_free()
void TEST_inti_and_free(size_t sizeof_data) {
	Vec *vec = Vec_init(sizeof_data);

	assert_eq(
	    vec != NULL && vec->size == 0 &&
		  vec->capacity == CCOLL_VEC_MIN_CAPACITY &&
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
	printf("here I am\n");
	assert_eq(
	    res == CCOLL_NULL_DATA,
	    "Vec_push() should fail if invalid arguments are provided"
	);

	char ch = 'a';
	res	  = Vec_push(NULL, &ch);
	assert_eq(
	    res == CCOLL_NULL,
	    "Vec_push() should fail if invalid arguments are provided"
	);

	res = Vec_push(NULL, NULL);
	assert_eq(
	    res == CCOLL_NULL,
	    "Vec_push() should fail if invalid arguments are provided"
	);

	res = Vec_push_front(vec, NULL);
	assert_eq(
	    res == CCOLL_NULL_DATA,
	    "Vec_push_front() should fail if invalid arguments are provided"
	);

	ch  = 'b';
	res = Vec_push_front(NULL, &ch);
	assert_eq(
	    res == CCOLL_NULL,
	    "Vec_push_front() should fail if invalid arguments are provided"
	);

	res = Vec_push_front(NULL, NULL);
	assert_eq(
	    res == CCOLL_NULL,
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
	    res == CCOLL_NULL, "Vec_free() should return CCOLL_INVALID_ARGUMENT"
	);

	vec = Vec_init_with(sizeof(char), 0);
	assert_eq(
	    vec == NULL,
	    "if min_capacity for Vec_init_with() is 0 it should return NULL"
	);
	res = Vec_free(vec);
	assert_eq(
	    res == CCOLL_NULL, "Vec_free() should return CCOLL_INVALID_ARGUMENT"
	);

	vec = Vec_init_with(0, 12);
	assert_eq(
	    vec == NULL,
	    "if sizeof_data for Vec_init_with() is 0 it should return NULL"
	);
	res = Vec_free(vec);
	assert_eq(
	    res == CCOLL_NULL, "Vec_free() should return CCOLL_INVALID_ARGUMENT"
	);

	vec = Vec_init_with(0, 0);
	assert_eq(
	    vec == NULL, "if sizeof_data and min_capacity for Vec_init_with() is "
			     "0 it should return NULL"
	);
	res = Vec_free(vec);
	assert_eq(
	    res == CCOLL_NULL, "Vec_free() should return CCOLL_INVALID_ARGUMENT"
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
		assert_eq(
		    first == NULL, "If you pop'ed only existing element second pop "
					 "should return NULL"
		);
		return;
	}
	assert_eq(
	    *first == number, "Vec_pop_front() & Vec_fill() with size:%ld", size
	);
}

int CALLBACK_after_element(
    void *element, size_t idx __attribute__((unused)), size_t element_size,
    CCOLL_OPERATION op __attribute__((unused))
) {
	log("CALLBACK_after_element() foo with data %s of size (%ld)",
	    (char *)element, element_size);
	return 0;
}

void TEST_after_element(
    size_t size,
    int (*fn)(void *, size_t idx, size_t element_size, CCOLL_OPERATION op)
) {
	Vec *vec = Vec_init_with((size + 1) * sizeof(char), size);
	if (vec == NULL) return;
	Vec_set_on_remove_callback(vec, fn);

	char *str = (char *)malloc((size + 1) * sizeof(char));
	generate_random_string(str, size);

	Vec_fill(vec, str);

	Vec_free(vec);
	return;
}

// Covers: Vec_set()
void TEST_vec_set() {
	Vec *vec  = Vec_init(sizeof(int));
	int data1 = 12;
	int data2 = 28;
	int res;

	res = Vec_set(vec, 1, &data1);
	assert_eq(
	    res == CCOLL_INVALID_ELEMENT,
	    "That operation would leave gap in data structure so Vec_set() "
	    "returns error"
	);
	assert_eq(
	    vec->size == 0, "just to check if it for sure din't set that data"
	);

	res = Vec_set(vec, 0, &data1);
	assert_eq(
	    res == CCOLL_SUCCESS,
	    "Vec_set() should prepend data if there isnt any and idx is 0"
	);
	assert_eq(
	    vec->size == 1, "just to check if it for sure added pthat data"
	);
	assert_eq(*(int *)vec->data == data1, "the data should be %i", data1);

	res = Vec_set(vec, 0, &data2);
	assert_eq(res == CCOLL_SUCCESS, "Vec_set() successful");
	assert_eq(vec->size == 1, "Just a check");
	assert_eq(*(int *)vec->data == data2, "the data should be %i", data2);

	res = Vec_set(vec, 2, &data1);
	assert_eq(
	    res == CCOLL_INVALID_ELEMENT,
	    "That operation would leave gap in data structure so Vec_set() "
	    "returns error"
	);
	assert_eq(
	    vec->size == 1, "just to check if it for sure din't set that data"
	);
}

int CALLBACK_vec_insert(
    void *data, size_t idx __attribute__((unused)), size_t element_size,
    CCOLL_OPERATION op __attribute__((unused))
) {
	log("size of data is " BLU "%ld" NOCOL " and data is (" BLU "%ld" NOCOL
	    ")",
	    element_size, *(long *)data);
	return CCOLL_SUCCESS;
}

// Covers: Vec_get_ptr(), Vec_insert(), Vec_set_after_callback(), Vec_free()
void TEST_vec_insert() {
	Vec *vec   = Vec_init(sizeof(long));
	long data1 = 123123;
	long data2 = 8888888;
	long data3 = 9;
	int res;

	res = Vec_insert(vec, 1, &data1);
	assert_eq(
	    res == CCOLL_INVALID_ELEMENT,
	    "the idx to Vec_insert() is intentionally invalid"
	);
	assert_eq(vec->size == 0, "Validation that vec didnt growth");

	printf("before\n");
	res = Vec_insert(vec, 0, &data1);
	printf("after\n");
	assert_eq(res == CCOLL_SUCCESS, "Vec_insert() test");
	assert_eq(
	    *(long *)Vec_get_ptr(vec, 0) == data1,
	    "Validation check if data is correct (%ld)", *(long *)Vec_get_ptr(vec, 0)
	);

	res = Vec_insert(vec, 0, &data2);
	assert_eq(res == CCOLL_SUCCESS, "Vec_insert() test");
	assert_eq(
	    *(long *)Vec_get_ptr(vec, 0) == data2,
	    "Validation check if data is correct (%ld)", *(long *)Vec_get_ptr(vec, 0)
	);

	res = Vec_insert(vec, 1, &data3);
	assert_eq(res == CCOLL_SUCCESS, "Vec_insert() test");
	assert_eq(
	    *(long *)Vec_get_ptr(vec, 1) == data3,
	    "Validation check if data is correct (%ld)", *(long *)Vec_get_ptr(vec, 1)
	);

	Vec_shrink(vec);

	assert_eq(*(long *)Vec_get_ptr(vec, 0) == data2, "Data integrity check");
	assert_eq(*(long *)Vec_get_ptr(vec, 1) == data3, "Data integrity check");
	assert_eq(*(long *)Vec_get_ptr(vec, 2) == data1, "Data integrity check");

	Vec_set_on_remove_callback(vec, CALLBACK_vec_insert);
	Vec_free(vec);
}

void TEST_vec_append(size_t size1, size_t size2) {
	log("Vec_append() for size1 %ld and size2 %ld", size1, size2);
	Vec *vecint1  = Vec_init_with(sizeof(int), size1);
	Vec *vecint2  = Vec_init_with(sizeof(int), size2);
	Vec *vecchar1 = Vec_init_with(sizeof(char), size1);
	Vec *vecchar2 = Vec_init_with(sizeof(char), size2);
	Vec *veclong  = Vec_init_with(sizeof(long), size1);

	if (!vecint1 || !vecint2 || !vecchar1 || !vecchar2 || !veclong) return;

	int ai  = 1;
	int bi  = 2;
	char ac = 'a';
	char bc = 'b';
	long al = 2323423423;

	Vec_fill(vecint1, &ai);
	Vec_fill(vecint2, &bi);
	Vec_fill(vecchar1, &ac);
	Vec_fill(vecchar2, &bc);
	Vec_fill(veclong, &al);

	int res;

	res = Vec_append(vecint1, veclong);
	assert_eq(res == CCOLL_ELEMENT_SIZE_MISMATCH, "Incompatybile vec types");
	assert_eq(vecint1->size == size1, "Safety check for size");
	assert_eq(veclong->size == size1, "Safety check for size");

	res = Vec_append(vecint1, vecint1);
	assert_eq(res == CCOLL_SUCCESS, "Incompatybile vec types");
	assert_eq(vecint1->size == (size1 + size1), "Safety check for size");
	assert_eq(
	    (*(int *)Vec_get_ptr(vecint1, 0) == ai) &&
		  (*(int *)Vec_get_ptr(vecint1, size1) == ai),
	    "data integrity check"
	);

	res = Vec_append(vecchar2, vecchar1);
	assert_eq(res == CCOLL_SUCCESS, "Incompatybile vec types");
	assert_eq(vecchar2->size == (size1 + size2), "Safety check for size");
	assert_eq(
	    (*(char *)Vec_get_ptr(vecchar2, 0) == bc) &&
		  (*(char *)Vec_get_ptr(vecchar2, size2) == ac),
	    "data integrity check"
	);

	Vec_shrink(vecint1);
	Vec_shrink(vecint2);
	Vec_shrink(vecchar2);

	res = Vec_append(vecint1, vecint2);
	assert_eq(res == CCOLL_SUCCESS, "Incompatybile vec types");
	assert_eq(
	    vecint1->size == (size1 + size1 + size2), "Safety check for size"
	);
	assert_eq(
	    (*(int *)Vec_get_ptr(vecint1, 0) == ai) &&
		  (*(int *)Vec_get_ptr(vecint1, size1 + size1) == bi),
	    "data integrity check"
	);

	res = Vec_append(veclong, veclong);
	assert_eq(res == CCOLL_SUCCESS, "Incompatybile vec types");
	assert_eq(veclong->size == (size1 + size1), "Safety check for size");
	assert_eq(
	    (*(long *)Vec_get_ptr(veclong, 0) == al) &&
		  (*(long *)Vec_get_ptr(veclong, size1) == al),
	    "data integrity check"
	);

	Vec_free(vecint1);
	Vec_free(vecint2);
	Vec_free(vecchar1);
	Vec_free(vecchar2);
	Vec_free(veclong);

	return;
}

// Covers: Vec_set_range(), Vec_get_ptr()
void TEST_vec_set_range(size_t size) {
	log("running with size:%ld", size);
	Vec *vec = Vec_init_with(sizeof(char), size);
	if (vec == NULL) return;

	char *data = malloc((size + 1) * sizeof(char));
	generate_random_string(data, size);

	int res;

	res = Vec_set_range(vec, data, 1, size);
	assert_eq(
	    res == CCOLL_INVALID_ELEMENT,
	    "Passed invalid values, should return errror"
	);
	assert_eq(vec->size == 0, "Additional check if vec size is valid");

	res = Vec_set_range(vec, data, 0, 0);
	assert_eq(res == CCOLL_SUCCESS, "Should be succesful");
	assert_eq(vec->size == 0, "But vec size should be 0");

	if (size > 1) {
		res = Vec_set_range(vec, data, 0, size / 2);
		assert_eq(res == CCOLL_SUCCESS, "should be succesful");
		assert_eq(vec->size == (size / 2), "checking size of the vec");
		assert_eq(
		    *(char *)Vec_get_ptr(vec, 0) == data[0],
		    "Additional data validation"
		);
	}

	res = Vec_set_range(vec, data, 0, size);
	assert_eq(res == CCOLL_SUCCESS, "Should be succesful");
	assert_eq(
	    vec->size == size, "size should be equal to %ld but is %ld", size,
	    vec->size
	);
	assert_eq(*(char *)vec->data == data[0], "Additional data validation");

	res = Vec_set_range(vec, data, size, size);
	assert_eq(res == CCOLL_SUCCESS, "Should be succesful");
	assert_eq(
	    vec->size == size + size, "size should be equal to %ld", size * 2
	);
	assert_eq(
	    *(char *)Vec_get_ptr(vec, 0) == data[0], "Additional data validation 1"
	);
	assert_eq(
	    *(char *)Vec_get_ptr(vec, size) == data[0], "Additional data validation 2"
	);

	Vec_free(vec);

	return;
}

static int clicked = 0;
int CALLBACK_after_fn_test(
    void *data, size_t idx __attribute__((unused)), size_t element_size,
    CCOLL_OPERATION op __attribute__((unused))
) {
	log("CALLBACK: %c <- %ld;", *(char *)data, element_size);
	clicked++;
	return 0;
}

void TEST_after_fn() {
	Vec *vec   = Vec_init(sizeof(char));
	char *data = (char *)malloc(8 * sizeof(char));
	generate_random_string(data, 7); // 31 bc 32 is null terminating

	Vec_push_range(vec, data, 7);
	Vec_set_on_remove_callback(vec, CALLBACK_after_fn_test);

	clicked = 0;
	Vec_set(vec, 0, &data[3]);
	assert_eq(clicked == 1, "Callback foo should click that");

	Vec_set_range(vec, data, 0, 3);
	assert_eq(
	    clicked == 4, "Callback function should increase click variable"
	);

	Vec_shrink(vec);
	Vec_fill(vec, &data[2]);
	assert_eq(
	    clicked == 4 + 7, "Callback function should increase click variable"
	);

	Vec_free(vec);
	assert_eq(
	    clicked == 4 + 7 + 7,
	    "Callback function should increase click variable"
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
	section("After Callback : (\e[38;5;1mManula check needed\e[0m)");
	TEST_after_element(0, CALLBACK_after_element);
	TEST_after_element(1, CALLBACK_after_element);
	TEST_after_element(2, CALLBACK_after_element);
	TEST_after_element(3, CALLBACK_after_element);
	TEST_after_element(10, CALLBACK_after_element);
	section("Set");
	TEST_vec_set();
	section("Insert");
	TEST_vec_insert();
	section("Append");
	TEST_vec_append(0, 33);
	TEST_vec_append(0, 0);
	TEST_vec_append(12, 0);
	TEST_vec_append(12, 33);
	TEST_vec_append(94234234, 99933);
	section("Set range");
	TEST_vec_set_range(0);
	TEST_vec_set_range(1);
	TEST_vec_set_range(100);
	TEST_vec_set_range(990090);
	section("Callback foo");
	TEST_after_fn();
	section("Remove range with callback");
	TEST_remove_range();
	TEST_remove_range_char();
	printf("\n");
	return 0;
}
