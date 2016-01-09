#include <stdio.h>
#include <stdlib.h>
#include "sb_sll.h"

int main(int argc, char ** argv)
{
	sb_sll * list;
	list = sll_init();
	int i;
	int * element = NULL;
	for (i = 0; i < 100000; i++)
	{
		element = (int *) malloc(sizeof(int));
		*element = i;
		sll_add_front(list, (void *) element);
	}

	while (!sll_empty(list)) {
		element = (int *) sll_front(list);
		printf("element: %d\n", *element);
		sll_free_front(list);
	}

	free(list);
	return EXIT_SUCCESS;
}
