#ifndef CCOLL_LLIST_H
#define CCOLL_LLIST_H

/*
 * LList (Linked List)
 *
 * Usage:
 *	not done yet
 */

// TODO: redo errors
// TODO: after making other ccoll's make sure that errors are consistent

// #ifndef CCOLL_ERROR_CODES_H
// #define CCOLL_ERROR_CODES_H
//
// #define CCOLL_SUCCESS 0
// #define CCOLL_ERROR -1
// #define CCOLL_INVALID_ARGUMENT -3
// #define CCOLL_OUT_OF_MEMORY -2
// #define CCOLL_NOT_ENOUGH_MEMORY_REQUESTED -4
// #define CCOLL_EMPTY -6
// #define CCOLL_PASSED_FOO_FAIL -7
// #define CCOLL_PASSED_FOO_FAIL_CONTINUED 7
//
// #endif

#include <stdbool.h>
#include <sys/types.h>

typedef struct Lli_node {
	// TODO: consider making *next union which
	// can contain pointer to Lli_node or Lli
	// then store in the last nodes pointer to the root
	struct Lli_node *next;
	char data[];
} Lli_node;

typedef struct Lli {
	Lli_node *head;
	Lli_node *tail;
	size_t size;
	size_t element_size;
} Lli;

// TODO: put here created foo's
// TODO: document these foo's

// Initializes Lli collection
//
// Returns: Pointer to Lli, NULL on failure
Lli *Lli_init(size_t sizeof_data);

//
//
//
Lli_node *Lli_create_node(void *data, size_t element_size);

//
//
//
int Lli_set(Lli *lli, size_t idx, void *data);

//
//
//
void *Lli_get_owned(Lli *lli, size_t idx);

//
//
//
int Lli_insert(Lli *lli, size_t idx, void *data);

//
//
//
int Lli_push(Lli *lli, void *data);

//
//
//
void *Lli_pop(Lli *lli);

//
//
//
int Lli_push_front(Lli *lli, void *data);

//
//
//
void *Lli_pop_front(Lli *lli);

#endif
