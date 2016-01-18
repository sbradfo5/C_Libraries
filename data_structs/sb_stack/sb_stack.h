#ifndef SB_STACK_H
#define SB_STACK_H

typedef struct sb_sll_s sb_sll;

typedef struct {
	sb_sll * S;
	int n;
} sb_stack;

sb_stack * stack_init();

void stack_destroy(sb_stack * instance);

int stack_size(sb_stack * instance);

int stack_empty(sb_stack * instance);

void * stack_top(sb_stack * instance);

void stack_push(sb_stack * instance, void * e);

void stack_pop(sb_stack * instance);

#endif
