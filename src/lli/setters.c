#include <stdlib.h>
#include <string.h>

#include "../../ccoll-codes.h"
#include "../../include/lli.h"

// TODO:TEST: Make test for that foo
int Lli_set(Lli *lli, size_t idx, void *data) {
	if (!lli) return CCOLL_INVALID_ARGUMENT;
	if (!lli->head) return CCOLL_INVALID_ARGUMENT;
	if (lli->size == 0) return CCOLL_EMPTY;
	if (!data) return CCOLL_INVALID_ARGUMENT;
	if (idx >= lli->size) return CCOLL_INVALID_ARGUMENT;

	Lli_node *iterator = lli->head;
	for (size_t i = 0; i < idx; i++) {
		iterator = iterator->next;
	}

	memcpy(iterator->data, data, lli->element_size);
	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
int Lli_insert(Lli *lli, size_t idx, void *data) {
	if (!lli) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;
	if (idx > lli->size) return CCOLL_INVALID_ARGUMENT;

	Lli_node *node = Lli_create_node(data, lli->element_size);
	if (!node) return CCOLL_OUT_OF_MEMORY;

	if (lli->size == 0) {
		lli->tail = node;
		lli->size++;
		return CCOLL_SUCCESS;
	}
	if (idx == 0) {
		node->next = lli->head;
		lli->head  = node;
		lli->size++;
		return CCOLL_SUCCESS;
	}

	Lli_node *iterator = lli->head;
	for (size_t i = 0; i < idx - 1; i++) {
		iterator = iterator->next;
	}

	node->next	   = iterator->next;
	iterator->next = node;

	if (idx == lli->size) lli->tail = node;

	lli->size++;
	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
int Lli_push(Lli *lli, void *data) {
	if (!lli) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;

	if (lli->size != 0 && !lli->tail) return CCOLL_INVALID_ARGUMENT;

	Lli_node *node = Lli_create_node(data, lli->element_size);
	if (!node) return CCOLL_OUT_OF_MEMORY;

	if (lli->size == 0) {
		lli->head = node;
		lli->tail = node;
	} else {
		lli->tail->next = node;
		lli->tail	    = node;
	}

	lli->size++;
	return CCOLL_SUCCESS;
}

// TODO:TEST: Make test for that foo
int Lli_push_front(Lli *lli, void *data) {
	if (!lli) return CCOLL_INVALID_ARGUMENT;
	if (!data) return CCOLL_INVALID_ARGUMENT;

	if (lli->size != 0 && !lli->head) return CCOLL_INVALID_ARGUMENT;

	Lli_node *node = Lli_create_node(data, lli->element_size);
	if (!node) return CCOLL_OUT_OF_MEMORY;

	if (lli->size == 0) {
		lli->head = node;
		lli->tail = node;
	} else {
		node->next = lli->head;
		lli->head  = node;
	}
	lli->size++;

	return CCOLL_SUCCESS;
}
