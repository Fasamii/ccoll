// TODO:IMPORTANT: make overflow checks for void *data in these foo's
#ifndef CCOLL_VEC_H
#define CCOLL_VEC_H

/*
 * Vec (Vector)
 *
 * Mind set:
 *	once some data is passed to vec it takes ownership of that data so using
 *old pointer to it as it int in vec structure is bad idea if you want to clone
 *	most foo's implement clone variant of itself
 *
 *	Vec automatically allocates memory but doesn't free it on its own
 *	but using Vec_init_with(), Vec_alloc() or Vec_reserve() is good idea if
 *	you care about performance
 *
 * Usage:
 *	#include "collections/vec.h" // To include vector
 *	Vec *vec = Vec_init(sizeof(<Type of data structure you want to story>))
 *
 * Example:
 *	#include "ccoll/vec.h"
 *
 *	// to create vector of struct storing two integers
 *	typedef struct Type {
 *		int a;
 *		int b;
 *	} Type;
 *	Vec *vec = Vec_init(sizeof(Type));
 *
 *	// to put data inside vec
 *	Type *t = (Type*)malloc(sizeof(Type));
 *	Vec_push(vec, t);
 */

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

#define CCOLL_VEC_MIN_CAPACITY 8

typedef struct Vec {
	size_t size;
	size_t capacity;
	size_t element_size;
	int (*on_remove)(
	    void *data, size_t idx, size_t element_size, CCOLL_OPERATION operation
	);
	void *data;
} Vec;

/////////////
// HELPERS //
/////////////

// TODO: make docs
#define Vec_idx_to_bytes(vec, idx) ((vec)->element_size * (idx))
#define Vec_bytes_to_idx(vec, idx) ((idx) / (vec)->element_size)

// TODO: make copy / pointer variants of foo's
// TODO: make unchecked variant of foo's

//////////
// INIT //
//////////

// Initializes Vec structure and returns pointer to it
//
// Returns:
// - Pointer to Vec
// - NULL on failure
Vec *Vec_init(size_t sizeof_data);

// Initializes Vec structure and ensures that its capacity
// is at least as provided then returns pointer to it
//
// Returns:
// - Pointer to Vec
// - NULL on failure
Vec *Vec_init_with(size_t sizeof_data, size_t min_capacity);

////////////
// MEMORY //
////////////

// TODO: define caller error codes e.g.: 0 for ok 1 for error but ignore and 2
// for terminate operation
// codes:
// 0 - nothing
// 1 - cancel element operation
// 2 - destroy entire vec
// IMPORTANT:TEST: make test's for on_remove error codes
// TODO: make better docs about operation
// Sets the foo that will be called on free for each element, not that is not
// destructor just a callback
//
// Can return:
// - CCOLL_SUCCESS
// - CCOLL_INVALID_ARGUMENT
int Vec_set_on_remove_callback(
    Vec *vec,
    int (*fn)(
	  void *, size_t idx, size_t element_size, CCOLL_OPERATION operation
    )
);

// Allocates enough memory to store provided number of elements
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_reserve(Vec *vec, const size_t idxs);

// Allocates exact memory to store provided number of elements
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_alloc(Vec *vec, const size_t idxs);

// Frees memory to fit Vec content
//
// Can return:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_shrink(Vec *vec);

// Frees memory to fit provided size
//
// Can return:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_NOT_ENOUGH_MEMORY_REQUESTED
// - CCOLL_INVALID_ARGUMENT
int Vec_shrink_to(Vec *vec, const size_t size);

// Frees Entire Vec structure
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_INVALID_ARGUMENT
int Vec_free(Vec *vec);

// Removes element at specified index
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_PASSED_FOO_FAIL
// - CCOLL_INVALID_ARGUMENT
int Vec_free_element(Vec *vec, size_t idx);

// Removes elements from specified range of the Vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_INVALID_ARGUMENT
int Vec_free_range(Vec *vec, size_t from_idx, size_t to_idx);

////////////////
// VEC-IDX-IO //
////////////////

// Sets data at specified idx
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT,
int Vec_set(Vec *vec, const size_t idx, const void *data);

// Set range of elements
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_set_range(
    Vec *vec, const void *data, size_t start_idx, const size_t quantity
);

// Returns pointer to data at specified Vec index
//
// Returns:
// - pointer to data
// - NULL on failure
static inline void *Vec_get(const Vec *vec, const size_t idx) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (idx >= vec->size) return NULL;

	return (char *)vec->data + (idx * vec->element_size);
}

// TODO: implement that foo
void *Vec_get_range(const Vec *vec, const size_t from_idx, size_t to_idx);

// TODO: read all Returns: and fix mistakes like not existing anymore errors
// etc...

////////////////
// VEC-POS-IO //
////////////////

// Push data to the end
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT,
int Vec_push(Vec *vec, const void *data);

// Push data to the front
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_push_front(Vec *vec, const void *data);

// Appends specified number of elements to Vec's back
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_push_range(Vec *vec, const void *data, size_t quantity);

// Appends specified number of elements to Vec's front
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_push_front_range(Vec *vec, const void *data, size_t quantity);

// Pop's data from the end and returns pointer to it
//
// Returns:
// - Pointer to data
// - NULL on failure
//
// !! You should free returned by data yourself !!
void *Vec_pop(Vec *vec);

// Pop's data from the beginning and returns pointer to it
//
// Returns:
// - Pointer to data
// - NULL on failure
//
// !! You should free returned by data yourself !!
void *Vec_pop_front(Vec *vec);

// TODO: implement that foo
void *Vec_pop_range(Vec *vec, size_t quantity);
// TODO: implement that foo
void *Vec_pop_front_range(Vec *vec, size_t quantity);

////////////////
// VEC-SHI-IO //
////////////////

// Inserts data at specified index of the Vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_insert(Vec *vec, const size_t idx, const void *data);

// Inserts data from specified start_idx
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_insert_range(
    Vec *vec, const void *data, const size_t start_idx, const size_t quantity
);

// Removes data from specified index
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_INVALID_ARGUMENT
// - CCOLL_EMPTY
int Vec_remove(Vec *vec, const size_t idx);

// Removes data from specified range of indexes
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_INVALID_ARGUMENT
int Vec_remove_range(Vec *vec, const size_t from_idx, const size_t to_idx);

////////////////
// VEC-TO-VEC //
////////////////

// Appends Vec with another Vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_append(Vec *base, const Vec *vec);

// Creates new Vec from two provided Vec's
//
// Returns:
// - Pointer to cloned appended vector
// - NULL on failure
Vec *Vec_append_clone(const Vec *vec1, const Vec *vec2);

// Splits Vec into two separate Vec's at idx Returns:
// - CCOLL_SUCCESS,
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_split(Vec *base, Vec **new_vec, const size_t idx);

// Clones Vec and splits that clone  into two separate Vec's at idx
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_INVALID_ARGUMENT
int Vec_split_clone(
    const Vec *base, Vec **new_vec1, Vec **new_vec2, const size_t idx
);

// Creates slice from provide Vec. Accepts negative indexes (Python style)
//
// Returns:
// - Pointer to slice of Vec
// - NULL on failure
Vec *Vec_slice(const Vec *vec, size_t from_idx, size_t to_idx);

//////////
// UTIL //
//////////

// Swaps two elements of the Vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_INVALID_ARGUMENT
int Vec_swap(Vec *vec, size_t idx1, size_t idx2);

// Fills Vec capacity with provided value
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_INVALID_ARGUMENT
int Vec_fill(Vec *vec, const void *data);

////////////////
// FUNCTIONAL //
////////////////

// Functional fashion foo that takes foo which will be called against all
// elements of Vec
//
// Passed *fn have special functionality based on return value:
//	0 - do nothing
//	1 - remove that element from vec
//	2 - skip next element
//	3 - destroy entire vec
//
// Returns: CCOLL_PASSED_FOO_FAIL_CONTINUED, CCOLL_INVALID_ARGUMENT,
// CCOLL_SUCCESS
int Vec_for_each(
    Vec *vec, int (*fn)(void *element, size_t idx, size_t element_size)
);

// Functional fashion foo that filters out which elements to clone to new Vec
//
// Returns: Vec that was created from filtered elements, NULL on failure
Vec *Vec_filter(
    Vec *vec, bool (*fn)(void *element, size_t idx, size_t element_size)
);

#endif
