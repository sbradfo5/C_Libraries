#include <stdio.h>
#include <stdlib.h>
#include "sb_stack.h"

int main(int argc, char ** argv)
{
	sb_stack * s;
	s = stack_init();

	// add a few thousand elements
	int i;
	int * e;
	for (i = 1; i <= 3000; i++) {
		e = (int *) malloc(sizeof(int));
		*e = i;
		stack_push(s, (void *) e); 
	}

	// pop them off one at a time and print them
	while (!stack_empty(s)) {
		e = (int *) stack_top(s);
		printf("Elem: %d\n", *e);
		stack_pop(s);
		free(e);
	}

	stack_destroy(s);
	return EXIT_SUCCESS;
}
