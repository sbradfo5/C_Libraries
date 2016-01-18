#include "sb_cll.h"
#include <stdlib.h>

sb_cll * cll_init() 
{
	sb_cll * instance;
	instance = (sb_cll *) malloc(sizeof(sb_cll));
	instance->cursor = NULL;
	return instance;
}

void cll_destroy(sb_cll * instance)
{
	while (!cll_empty(instance)) {
		cll_remove(instance);
	}
	free(instance);
}

int cll_empty(sb_cll * instance)
{
	return instance->cursor == NULL;
}

void * cll_front(sb_cll * instance)
{
	if (!cll_empty(instance)) {
		return instance->cursor->next->elem;
	}
	else {
		return NULL;
	}
}

void * cll_back(sb_cll * instance)
{
	if (!cll_empty(instance)) {
		return instance->cursor->elem;
	}
	else {
		return NULL;
	}
}

void cll_advance(sb_cll * instance)
{
	instance->cursor = instance->cursor->next;
	return;
}

void cll_add(sb_cll * instance, void * e)
{
	sb_cll_node * v = (sb_cll_node *) malloc(sizeof(sb_cll_node));
	v->elem = e;
	if (instance->cursor == NULL) {
		v->next = v;
		instance->cursor = v;
	}
	else {
		v->next = instance->cursor->next;
		instance->cursor->next = v;
	}
	return;
}

void cll_remove(sb_cll * instance)
{
	// check if there is something to remove
	if (cll_empty(instance)) {
		return;
	}

	// remove element
	sb_cll_node * old = instance->cursor->next;
	if (old == instance->cursor) {
		instance->cursor = NULL;
	}
	else {
		instance->cursor->next = old->next;
	}
	free(old);
	return;
}
