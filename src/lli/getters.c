#include <stdlib.h>
#include <string.h>

#include "../../include/lli.h"

// TODO:TEST: Make test for that foo
void *Lli_get_owned(Lli *lli, size_t idx) {
	if (!lli) return NULL;
	if (!lli->head) return NULL;
	if (lli->size == 0) return NULL;
	if (idx >= lli->size) return NULL;

	void *data = (void *)malloc(lli->element_size);
	if (!data) return NULL;

	if (idx == lli->size - 1 && lli->tail) {
		memcpy(data, lli->tail->data, lli->element_size);
		return data;
	}

	Lli_node *iterator = lli->head;
	for (size_t i = 0; i < idx; i++) {
		iterator = iterator->next;
	}

	memcpy(data, iterator->data, lli->element_size);

	return data;
}

// TODO:TEST: Make test for that foo
void *Lli_get_pointer(Lli *lli, size_t idx) {
	if (!lli) return NULL;
	if (!lli->head) return NULL;
	if (lli->size == 0) return NULL;
	if (idx >= lli->size) return NULL;

	if (idx == lli->size - 1 && lli->tail) {
		return lli->tail->data;
	}

	Lli_node *iterator = lli->head;
	for (size_t i = 0; i < idx; i++) {
		iterator = iterator->next;
	}

	return iterator->data;
}

// TODO:TEST: Make test for that foo
void *Lli_pop(Lli *lli) {
	if (!lli) return NULL;
	if (lli->size == 0) return NULL;
	if (!lli->tail) return NULL;

	void *data = (void *)malloc(lli->element_size);
	if (!data) return NULL;

	if (lli->size == 1) {
		memcpy(data, lli->tail->data, lli->element_size);
		free(lli->tail);
		lli->tail = NULL;
		lli->head = NULL;
		lli->size = 0;
		return data;
	}

	Lli_node *iterator = lli->head;
	while (iterator->next != lli->tail)
		iterator = iterator->next;

	memcpy(data, lli->tail->data, lli->element_size);
	free(lli->tail);

	iterator->next = NULL;
	lli->tail	   = iterator;
	lli->size--;

	return data;
}

// TODO:TEST: Make test for that foo
void *Lli_pop_front(Lli *lli) {
	if (!lli) return NULL;
	if (lli->size == 0) return NULL;
	if (!lli->head) return NULL;

	void *data = (void *)malloc(lli->element_size);
	if (!data) return NULL;

	memcpy(data, lli->head->data, lli->element_size);

	if (lli->size == 1) {
		free(lli->head);
		lli->head = NULL;
		lli->tail = NULL;
		lli->size = 0;
	} else {
		Lli_node *old_head = lli->head;
		lli->head		 = lli->head->next;
		free(old_head);
		lli->size--;
	}

	return data;
}
