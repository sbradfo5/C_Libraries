#include "sb_deq.h"
#include "sb_dll.h"
#include <stdlib.h>

sb_deq * deq_init()
{
	sb_deq * instance;
	instance = (sb_deq *) malloc(sizeof(sb_deq));
	instance->D = dll_init();
	instance->size = 0;
	return instance;
}

void deq_destroy(sb_deq * instance)
{
	dll_destroy(instance->D);
	free(instance);
}

int deq_size(sb_deq * instance) 
{
	return instance->size;
}

int deq_empty(sb_deq * instance)
{
	return instance->size == 0;
}

void * deq_front(sb_deq * instance)
{
	if (deq_empty(instance)) {
		return NULL;
	}
	return dll_front(instance->D);
}

void * deq_back(sb_deq * instance)
{
	if (deq_empty(instance)) {
		return NULL;
	}
	return dll_back(instance->D);
}

void deq_insert_front(sb_deq * instance, void * e)
{
	dll_add_front(instance->D, e);
	instance->size++;
}

void deq_insert_back(sb_deq * instance, void * e)
{
	dll_add_back(instance->D, e);
	instance->size++;
}

void deq_remove_front(sb_deq * instance)
{
	if (deq_empty(instance)) {
		return;
	}
	dll_remove_front(instance->D);
	instance->size--;
}

void deq_remove_back(sb_deq * instance)
{
	if (deq_empty(instance)){
		return;
	}
	dll_remove_back(instance->D);
	instance->size--;
}
