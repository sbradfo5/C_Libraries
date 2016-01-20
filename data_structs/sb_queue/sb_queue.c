#include "sb_queue.h"
#include "sb_cll.h"
#include <stdlib.h>

sb_queue * queue_init()
{
	sb_queue * instance;
	instance = (sb_queue *) malloc(sizeof(sb_queue));
	instance->C = cll_init();
	instance->size = 0;
	return instance;
}

void queue_destroy(sb_queue * instance)
{
	cll_destroy(instance->C);
	free(instance);
}

int queue_size(sb_queue * instance)
{
	return instance->size;
}

int queue_empty(sb_queue * instance)
{
	return instance->size == 0;
}

void * queue_front(sb_queue * instance)
{
	if (queue_empty(instance)) {
		return NULL;
	}
	return cll_front(instance->C);
}

void queue_enqueue(sb_queue * instance, void * e)
{
	cll_add(instance->C, e);
	cll_advance(instance->C);
	instance->size++;
}

void queue_dequeue(sb_queue * instance)
{
	if (queue_empty(instance)) {
		return;
	}
	cll_remove(instance->C);
	instance->size--;
}
