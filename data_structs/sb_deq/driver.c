#include <stdio.h>
#include <stdlib.h>
#include "sb_deq.h"

int main(int argc, char ** argv)
{
	// create the dequeue
	sb_deq * d;

	d = deq_init();

	// add a few thousand elements to the front
	int i;
	int * e;
	for (i = 1; i <= 3000; i++) {
		e = (int *) malloc(sizeof(int));
		*e = i;
		deq_insert_front(d, (void *) e);
	}

	// go through them, print, and free elements
	while (!deq_empty(d)) {
		e = (int *) deq_front(d);
		deq_remove_front(d);
		printf("front: %d\n", *e);
		free(e);
	}

	// add a few thousand elements to the back
	for (i = 1; i <= 3000; i++) {
		e = (int *) malloc(sizeof(int));
		*e = i;
		deq_insert_back(d, (void *) e);
	}

	// go through them, print, and free elements
	while (!deq_empty(d)) {
		e = (int *) deq_back(d);
		deq_remove_back(d);
		printf("back: %d\n", *e);
		free(e);
	}

	// free the memory of the dequeue
	deq_destroy(d);
	return 0;
}
