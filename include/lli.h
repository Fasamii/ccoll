#ifndef CCOLL_LLIST_H
#define CCOLL_LLIST_H

/*
 * LList (Linked List)
 *
 * Usage:
 *	not done yet
 */

#include <stdbool.h>
#include <sys/types.h>

typedef struct Lli_node {
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

Lli *Lli_init(size_t sizeof_data);

Lli_node *Lli_create_node(void *data, size_t element_size);

int Lli_set(Lli *lli, size_t idx, void *data);

void *Lli_get_owned(Lli *lli, size_t idx);

int Lli_insert(Lli *lli, size_t idx, void *data);

int Lli_push(Lli *lli, void *data);

void *Lli_pop(Lli *lli);

int Lli_push_front(Lli *lli, void *data);

void *Lli_pop_front(Lli *lli);

#endif
