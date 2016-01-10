#include "sb_dll.h"
#include <stdlib.h>

sb_dll * dll_init()
{
	sb_dll * instance;
	instance = (sb_dll *) malloc(sizeof(sb_dll));
	instance->header = (sb_dll_node *) malloc(sizeof(sb_dll_node));
	instance->trailer = (sb_dll_node *) malloc(sizeof(sb_dll_node));
	instance->header->next = instance->trailer;
	instance->trailer->prev = instance->header;
	return instance;
}

void dll_destroy(sb_dll * instance)
{
	while (!dll_empty(instance)) {
		dll_remove_front(instance);
	}
	
	free(instance->header);
	free(instance->trailer);
	free(instance);
}

int dll_empty(sb_dll * instance)
{
	return instance->header->next == instance->trailer;
}

void * dll_front(sb_dll * instance)
{
	if (dll_empty(instance)) {
		return NULL;
	}
	return instance->header->next->elem;
}

void * dll_back(sb_dll * instance)
{
	if (dll_empty(instance)) {
		return NULL;
	}
	return instance->trailer->prev->elem;
}

void dll_add_front(sb_dll * instance, void * e)
{
	dll_add_before(instance->header->next, e);
}

void dll_add_back(sb_dll * instance, void * e)
{
	dll_add_before(instance->trailer, e);
}

void dll_add_before(sb_dll_node * v, void * e)
{
	sb_dll_node * u;
	u = malloc(sizeof(sb_dll_node));
	u->elem = e;
	u->next = v;
	u->prev = v->prev;
	v->prev->next = u;
	v->prev = u;
}

void dll_remove(sb_dll_node * v)
{
	sb_dll_node * u = v->prev;
	sb_dll_node * w = v->next;
	u->next = w;
	w->prev = u;
	free(v);
}

void dll_remove_front(sb_dll * instance)
{
	dll_remove(instance->header->next);
}

void dll_remove_back(sb_dll * instance)
{
	dll_remove(instance->trailer->prev);
}

void dll_foreach(sb_dll * instance, void (*call_back)(void * d, void * e), void * data)
{
	sb_dll_node * pos;
	pos = instance->header;
	while (pos->next != instance->trailer) {
		pos = pos->next;
		call_back(data, pos->elem);
	}
}
