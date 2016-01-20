#include "sb_queue.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	// create the queue
	sb_queue * q;
	q = queue_init();

	// add a few thousand elements
	int i;
	int * e;
	for (i = 1; i <= 3000; i++) {
		e = (int *) malloc(sizeof(int));
		*e = i;
		queue_enqueue(q, (void *) e);
	}

	// go through each element and print
	while (!queue_empty(q)) {
		e = (int *) queue_front(q);
		queue_dequeue(q);
		printf("elem: %d\n", *e);
		free(e);
	}

	queue_destroy(q);
	return EXIT_SUCCESS;
}
