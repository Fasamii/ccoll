#include "../../include/lli.h"

#include <stdlib.h>
#include <string.h>

// TODO:TEST: Make test for that foo
Lli *Lli_init(size_t sizeof_data) {
	Lli *lli = (Lli*)malloc(sizeof(Lli));
	if (!lli) return NULL;
	lli->element_size = sizeof_data;
	lli->size = 0;
	lli->head = NULL;
	lli->tail = NULL;
	return lli;
}

// TODO:TEST: Make test for that foo
Lli_node *Lli_create_node(void *data, size_t element_size) {
	Lli_node *lli_node = (Lli_node*)malloc(sizeof(Lli_node) + element_size);
	if (!lli_node) return NULL;
	lli_node->next = NULL;
	memcpy(lli_node->data, data, element_size);
	return lli_node;
}
