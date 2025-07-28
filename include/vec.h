// TODO:IMPORTANT: make overflow checks for void *data in these foo's

// TODO: rewrite below comment
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

#ifndef CCOLL_VEC_H
#define CCOLL_VEC_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

#include "../ccoll-codes.h"

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

// Calculates how much bytes passed amount of idxs will take
#define Vec_idx_to_bytes(vec, idxs) ((vec)->element_size * (idxs))

// Calculates how much idxs will fit in specified amount of idxs
#define Vec_bytes_to_idx(vec, idxs) ((idxs) / (vec)->element_size)

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

// sets on_remove callback for the vec
//
// codes:
// 1. CCOLL_CALLBACK_NOTHING - do nothing
// 2. CCOLL_CALLBACK_CANCEL - cancel operation
// 3. CCOLL_CALLBACK_DESTROY_VEC - destroy entire vec
//
// Can return:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_FN
int Vec_set_on_remove_callback(
    Vec *vec,
    int (*fn)(
	  void *, size_t idx, size_t element_size, CCOLL_OPERATION operation
    )
);

// Allocates enough memory to store provided additional number of elements
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_OUT_OF_MEMORY
int Vec_reserve(Vec *vec, const size_t idxs);

// Allocates enough memory to store provided number of elements
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NOT_ENOUGH_MEMORY_REQUESTED
// - CCOLL_OUT_OF_MEMORY
int Vec_reserve_entire(Vec *vec, const size_t idxs);

// Allocates memory for exact amount of elements
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_OUT_OF_MEMORY
int Vec_alloc(Vec *vec, const size_t idxs);

// Frees memory to fit Vec contents
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_OUT_OF_MEMORY
int Vec_shrink(Vec *vec);

// Frees memory to fit provided size
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NOT_ENOUGH_MEMORY_REQUESTED
// - CCOLL_OUT_OF_MEMORY
int Vec_shrink_to(Vec *vec, const size_t size);

// Frees entire Vec structure
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
int Vec_free(Vec *vec);

// Frees specified amount of idxs from Vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NOT_ENOUGH_MEMORY_REQUESTED
// - CCOLL_OUT_OF_MEMORY
int Vec_free_ammount(Vec *vec, size_t idxs);

////////////////
// VEC-IDX-IO //
////////////////

// Sets data at specified idx
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_INVALID_ELEMENT
// - CCOLL_OVERFLOW
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_CANCELED
// - CCOLL_DESTROYED
int Vec_set(Vec *vec, const size_t idx, const void *data);

// Set range of elements
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_INVALID_ELEMENT
// - CCOLL_OVERFLOW
// - CCOLL_OUT_OF_MEMORY
// - CCOLL_SUCCESS_WITH_CANCELED
// - CCOLL_DESTROYED
int Vec_set_range(
    Vec *vec, const void *data, size_t start_idx, const size_t quantity
);

// Gets pointer to element at specified idx
//
// Returns:
// - pointer to data
// - undefined behavior
static inline void *Vec_get_unchecked_ptr(const Vec *vec, const size_t idx) {
	return (char *)vec->data + (idx * vec->element_size);
}

// Gets pointer to element at specified idx and rus checks if provided data is
// valid
//
// Returns:
// - pointer to data
// - NULL on failure
static inline void *Vec_get_ptr(const Vec *vec, const size_t idx) {
	if (!vec) return NULL;
	if (!vec->data) return NULL;
	if (idx >= vec->size) return NULL;

	return Vec_get_unchecked_ptr(vec, idx);
}

// Clones vec element from specified idx and returns pointer to it
//
// Returns:
// - pointer to data
// - undefined behavior
//
// !! You are responsible for freeing returned data !!
void *Vec_get_clone_unchecked(const Vec *vec, const size_t idx);

// Clones vec element from specified idx and returns pointer to it
//
// Returns:
// - Pointer to data
// - NULL on failure
//
// !! You are responsible for freeing returned data !!
void *Vec_get_clone(const Vec *vec, const size_t idx);

////////////////
// VEC-POS-IO //
////////////////

// Push data to the end
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_OVERFLOW
// - CCOLL_OUT_OF_MEMORY
int Vec_push(Vec *vec, const void *data);

// Push data to the front
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_OVERFLOW
// - CCOLL_OUT_OF_MEMORY
int Vec_push_front(Vec *vec, const void *data);

// Appends specified number of elements to Vec's back
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_OVERFLOW
// - CCOLL_OUT_OF_MEMORY
int Vec_push_range(Vec *vec, const void *data, size_t quantity);

// Appends specified number of elements to Vec's front
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_OVERFLOW
// - CCOLL_OUT_OF_MEMORY
int Vec_push_front_range(Vec *vec, const void *data, size_t quantity);

// Pop's data from the end and returns pointer to it
//
// Returns:
// - Pointer to data
// - NULL on failure
//
// !! You are responsible for freeing returned data !!
void *Vec_pop(Vec *vec);

// Pop's data from the beginning and returns pointer to it
//
// Returns:
// - Pointer to data
// - NULL on failure
//
// !! You are responsible for freeing returned data !!
void *Vec_pop_front(Vec *vec);

////////////////
// VEC-SHI-IO //
////////////////

// Inserts data at specified index of the Vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_INVALID_ELEMENT
// - CCOLL_OUT_OF_MEMORY
int Vec_insert(Vec *vec, const size_t idx, const void *data);

// Inserts data from specified start_idx
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_INVALID_ELEMENT
// - CCOLL_OVERFLOW
// - CCOLL_OUT_OF_MEMORY
int Vec_insert_range(
    Vec *vec, const void *data, const size_t start_idx, const size_t quantity
);

// Removes data from specified index
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_EMPTY
// - CCOLL_INVALID_ELEMENT
// - CCOLL_CANCELED
// - CCOLL_DESTROYED
int Vec_remove(Vec *vec, const size_t idx);

// Removes data from specified range of indexes
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
// - CCOLL_INVALID_RANGE
// - CCOLL_INVALID_ELEMENT
// - CCOLL_SUCCESS_WITH_CANCELED
// - CCOLL_SUCCESS_NO_REMOVED_MEMORY_WITH_CANCELED
// - CCOLL_DESTROYED
int Vec_remove_range(Vec *vec, const size_t from_idx, const size_t to_idx);

// TODO: implement on_remove for that
//
// Removes all elements from the vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
int Vec_remove_all(Vec *vec);

////////////////
// VEC-TO-VEC //
////////////////

// Appends Vec with another Vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_OVERFLOW
// - CCOLL_ELEMENT_SIZE_MISMATCH
// - CCOLL_OUT_OF_MEMORY
int Vec_append(Vec *base, const Vec *vec);

// Creates new Vec from two provided Vec's
//
// Returns:
// - Pointer to cloned appended vector
// - NULL on failure
Vec *Vec_append_clone(const Vec *vec1, const Vec *vec2);

// Splits Vec into two separate Vec's at idx. Foo automatically Initializes
// new_vecs
//
// Returns:
// - CCOLL_SUCCESS,
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_INVALID_ELEMENT
// - CCOLL_OUT_OF_MEMORY
int Vec_split(Vec *base, Vec **new_vec, const size_t idx);

// Clones Vec and splits that clone  into two separate Vec's at idx. Foo
// automatically Initializes new_vecs
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_INVALID_ELEMENT
// - CCOLL_OUT_OF_MEMORY
int Vec_split_clone(
    const Vec *base, Vec **new_vec1, Vec **new_vec2, const size_t idx
);

// Slices passed vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_INVALID_RANGE
// - CCOLL_INVALID_ELEMENT
int Vec_slice(Vec *vec, size_t from_idx, size_t to_idx);

// Creates slice from provided Vec
//
// Returns:
// - Pointer to slice of Vec
// - NULL on failure
Vec *Vec_slice_clone(const Vec *vec, size_t from_idx, size_t to_idx);

//////////
// UTIL //
//////////

// Swaps two elements of the Vec
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_INVALID_ELEMENT
int Vec_swap(Vec *vec, size_t idx1, size_t idx2);

// Fills Vec capacity with provided value
//
// Returns:
// - CCOLL_SUCCESS
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_DATA
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
// Returns:
// - CCOLL_NULL
// - CCOLL_NULL_INTERNAL_DATA
// - CCOLL_NULL_FN
// - CCOLL_DESTROYED
int Vec_for_each(
    Vec *vec, int (*fn)(void *element, size_t idx, size_t element_size)
);

// Functional fashion foo that filters out which elements to clone to new
// Vec
//
// Returns:
// - Vec that was created from filtered elements
// - NULL on failure
Vec *Vec_filter(
    Vec *vec, bool (*fn)(void *element, size_t idx, size_t element_size)
);

#endif
