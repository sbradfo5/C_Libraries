#include "sb_sll.h"
#include <stdlib.h>

sb_sll * sll_init()
{
	sb_sll * instance;
	instance = (sb_sll *) malloc(sizeof(sb_sll) * 1);
	instance->head = NULL;
	instance->size = 0;
	return instance;
}

int sll_empty(sb_sll * instance)
{
	return instance->head == NULL;
}

int sll_size(sb_sll * instance)
{
	return instance->size;
}

void * sll_front(sb_sll * instance)
{
	if (sll_empty(instance)) {
		return NULL;
	}
	return instance->head->elem;
}

void sll_add_front(sb_sll * instance, void * elem)
{
	sb_sll_node * v = (sb_sll_node *) malloc(sizeof(sb_sll_node) * 1);
	v->elem = elem;
	v->next = instance->head;
	instance->head = v;
	instance->size++;
}

void sll_remove_front(sb_sll * instance)
{
	sb_sll_node * old = instance->head;
	instance->head = old->next;
	free(old);
	instance->size--;
}

void sll_destroy(sb_sll * instance)
{
	while(!sll_empty(instance)) {
		sll_remove_front(instance);
	}
	
	free(instance);
}
