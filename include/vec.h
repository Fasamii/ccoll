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

#ifndef CCOLL_ERROR_CODES_H
#define CCOLL_ERROR_CODES_H

#define CCOLL_SUCCESS 0
#define CCOLL_ERROR -1

#define CCOLL_OUT_OF_MEMORY -2

#define CCOLL_INVALID_ARGUMENT -3
#define CCOLL_NOT_ENOUGH_MEMORY_REQUESTED -4
#define CCOLL_NULL_VEC -5
#define CCOLL_EMPTY -6

#define CCOLL_DESTRUCTOR_FOO_FAIL -7
#define CCOLL_DESTRUCTOR_FOO_FAIL_CONTINUED 7

#endif

#include <sys/types.h>

#define VEC_MIN_CAPACITY 8

typedef struct Vec {
	size_t size;
	size_t capacity;
	size_t element_size;
	void *data;
	int (*destructor)(void *element);
} Vec;

// Initializes Vec structure and
// returns pointer to it
//
// no error messages since it returns pointer only NULL
Vec *Vec_init(size_t sizeof_data);

// Initializes Vec structure and ensures that its capacity
// is at least as provided then returns pointer to it
//
// No error messages since it returns pointer only NULL
Vec *Vec_init_with(size_t sizeof_data, size_t min_capacity);

// Sets the destruktor or cleanup foo for Vec
// (used within Vec_free())
//
// Can return: CCOLL_INVALID_ARGUMENT and CCOLL_SUCCESS
int Vec_set_destructor(Vec *vec, int (*destructor)(void *));

// Makes sure that Vec have enough capacity to
// store provided size
//
// Can return: CCOLL_INVALID_ARGUMENT,
// CCOLL_OUT_OF_MEMORY and
// CCOLL_SUCCESS
int Vec_reserve(Vec *vec, size_t idxs);

// Allocates exact amount of memory as
// provided
//
// Can return:
// CCOLL_INVALID_ARGUMENT,
// CCOLL_OUT_OF_MEMORY or
// CCOLL_SUCCESS
int Vec_alloc(Vec *vec, size_t idxs);

// Frees vec structure leaving
// its elements
//
// Can return:
// CCOLL_INVALID_ARGUMENT,
// CCOLL_SUCCESS
int Vec_free_shallow(Vec *vec);

// Frees entire Vec
// with its elements
//
// Can return:
// CCOLL_INVALID_ARGUMENT,
// CCOLL_SUCCESS
int Vec_free(Vec *vec);

// Removes elements from specified range of the Vec if range
// is not valid it just ignores it
//
// Returns: CCOLL_INVALID_ARGUMENT, CCOLL_SUCCESS
int Vec_free_range(Vec *vec, size_t from_idx, size_t to_idx);

// Removes element at specified index
//
//
int Vec_free_element(Vec *vec, size_t idx);

// Removes data from specified index of
// the Vec (Use only if data doesn't have destructor)
//
// Can return: CCOLL_SUCCESS,
// CCOLL_INVALID_ARGUMENT,
// CCOLL_EMPTY
int Vec_remove(Vec *vec, size_t idx);

// Frees memory to fit
// Vec content
//
// Can return:
// CCOLL_INVALID_ARGUMENT,
// CCOLL_OUT_OF_MEMORY,
// CCOLL_SUCCESS
int Vec_shrink(Vec *vec);

// Frees memory to fit provided size
//
// Can return: CCOLL_INVALID_ARGUMENT,
// CCOLL_NOT_ENOUGH_MEMORY_REQUESTED,
// CCOLL_OUT_OF_MEMORY, CCOLL_SUCCESS
int Vec_shrink_to(Vec *vec, size_t size);

// Sets vec data at specified index, also returns pointer to
// data that was replaced with Vec_set() if some actually was
// replaced, if you want to ignore replaced free it at least
//
// Can return: CCOLL_INVALID_ARGUMENT,
// CCOLL_OUT_OF_MEMORY, CCOLL_SUCCESS
int Vec_set(Vec *vec, size_t idx, void *data);

// Returns pointer to data at
// specified Vec index
//
// No error messages since it returns pointer only NULL
void *Vec_get(Vec *vec, size_t idx);

// Pushes data to the end of Vector
//
// Can return: CCOLL_SUCCESS,
// CCOLL_INVALID_ARGUMENT,
// CCOLL_OUT_OF_MEMORY
int Vec_push(Vec *vec, void *data);

// Pops data from the
// end of Vector and
// returns pointer to it
//
// No error messages since it returns pointer only NULL
//
// You should free returned data
void *Vec_pop(Vec *vec);

// Pushes data to the beginning of the
// Vec
//
// Can return: CCOLL_INVALID_ARGUMENT,
// CCOLL_OUT_OF_MEMORY, CCOLL_SUCCESS
int Vec_push_back(Vec *vec, void *data);

// Pops data from the beginning of the
// Vector and returns pointer to it
//
// No error messages since it returns pointer only NULL
//
// You should free returned
// data
void *Vec_pop_back(Vec *vec);

// Inserts data to specified index of the Vec
//
// Can return: CCOLL_INVALID_ARGUMENT,
// CCOLL_OUT_OF_MEMORY, CCOLL_SUCCESS
int Vec_insert(Vec *vec, size_t idx, void *data);

// Appends Vec with another Vec
//
// Can return: CCOLL_SUCCESS,
// CCOLL_INVALID_ARGUMENT,
// CCOLL_OUT_OF_MEMORY
int Vec_append(Vec *base, Vec *vec);

// Creates new Vector from two Vec's
// provided
//
// No error messages since it returns pointer only NULL
Vec *Vec_append_clone(Vec *vec1, Vec *vec2);

// Splits Vec into two separate Vec's at provided
// index the new_vec variable doesn't need to be
// initialized
//
// Returns: CCOLL_INVALID_ARGUMENT, CCOLL_SUCCESS,
// CCOLL_OUT_OF_MEMORY
int Vec_split(Vec *base, Vec *new_vec, size_t idx);

// Clones Vec and splits that clone into two separate Vec's at specified
// index new_vec1 and new_vec2 doesn't have to be initialized
//
// Returns: CCOLL_INVALID_ARGUMENT, CCOLL_SUCCESS,
// CCOLL_OUT_OF_MEMORY
int Vec_split_clone(Vec *base, Vec *new_vec1, Vec *new_vec2, size_t idx);

// Swaps two elements of the Vec
int Vec_swap(Vec *vec, size_t idx1, size_t idx2);

// Sets specified number of elements keep in mind that you have to provide
// list of pointers as data pointer
int Vec_set_range(Vec *vec, void **data, size_t start_idx, size_t quantity);

// Inserts data at specified start_idx of vector
int Vec_insert_range(Vec *vec, void **data, size_t start_idx, size_t quantity);

// Appends specified number of elements to Vec keep in mind
// that you need to provide list of pointer as data pointer
int Vec_push_range(Vec *vec, void **data, size_t quantity);

int _Vec_realloc_checked(Vec *vec, size_t capacity_target);

#endif
