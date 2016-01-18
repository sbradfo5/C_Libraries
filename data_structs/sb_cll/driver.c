#include <stdio.h>
#include <stdlib.h>
#include "sb_cll.h"

int main(int argc, char ** argv) 
{
	int * e;
	sb_cll_node * begin;

	// test creation
	sb_cll * list;
	list = cll_init();

	// add a few thousand elements
	int i;
	for (i = 0; i < 3000; i++) {
		e = (int *) malloc(sizeof(int));
		*e = i;
		cll_add(list, e);
	}

	// test iterating elements
	begin = list->cursor;
	do {
		e = cll_front(list);
		printf("element: %d\n", *e);
		cll_advance(list);
	} while (list->cursor != begin);

	// test iterating and removing elements at the same time
	while (!cll_empty(list)) {
		e = cll_front(list);
		cll_remove(list);
		free(e);
	}

	cll_destroy(list);
	return EXIT_SUCCESS;
}
