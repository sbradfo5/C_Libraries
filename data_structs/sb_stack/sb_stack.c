#include "sb_stack.h"
#include "sb_sll.h"
#include <stdlib.h>

sb_stack * stack_init()
{
	sb_stack * instance;
	instance = (sb_stack *) malloc(sizeof(sb_stack));
	instance->S = sll_init();
	instance->n = 0;
	return instance;
}

void stack_destroy(sb_stack * instance)
{
	while (!stack_empty(instance)) {
		stack_pop(instance);
	}
	sll_destroy(instance->S);
	free(instance);
}

int stack_size(sb_stack * instance)
{
	return instance->n;
}

int stack_empty(sb_stack * instance)
{
	return instance->n == 0;
}

void * stack_top(sb_stack * instance)
{
	if (stack_empty(instance)) {
		return NULL;
	}
	else {
		return sll_front(instance->S);
	}
}

void stack_push(sb_stack * instance, void * e)
{
	instance->n++;
	sll_add_front(instance->S, e);
	return;
}

void stack_pop(sb_stack * instance)
{
	if (stack_empty(instance)) {
		return;
	}
	instance->n--;
	sll_remove_front(instance->S);
	return;
}
