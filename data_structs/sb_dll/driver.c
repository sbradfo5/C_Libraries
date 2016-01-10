#include "sb_dll.h"
#include <stdio.h>
#include <stdlib.h>

void my_call_back(void * myData, void * element)
{
	int * e = (int *) element;
	printf("callback: %d\n", *e);
	free(e);
}

int main(int argc, char ** argv)
{
	sb_dll * list;
	sb_dll_node * node;
	int i;
	int * e;

	// create list
	list = dll_init();
	
	// add elements
	for (i = 1; i <= 10; i++) {
		e = (int *) malloc(sizeof(int));
		*e = i;
		dll_add_back(list, (void *) e);
	}
	
	/* code to traverse list forward */
	printf("iter1: ");
	node = dll_begin(list);
	while (node != dll_end(list)) {
		e = (int *) dll_get_node_val(node);
		printf("%d ", *e);
		node = node->next;
	}
	printf("\n");

	/* code to traverse list backward: little more clunky */
	printf("iter2: ");

	// the end is list->trailer, which has no element
	node = dll_end(list);

	// so go back one
	node = node->prev;

	// and go backwards until list->header
	while (node != list->header) {
		e = (int *) dll_get_node_val(node);
		printf("%d ", *e);
		node = node->prev;
	}
	printf("\n");
		
	
	// iterator code to traverse list and change an element
	node = dll_begin(list);
	while (node != dll_end(list)) {
		e = (int *) dll_get_node_val(node);
		if (*e == 5) {
			*e = 20;
		}
		node = node->next;
	}

	// using foreach and call back code to print elements and free them
	dll_foreach(list, my_call_back, NULL);

	// destroy nodes and list itself
	dll_destroy(list);
	return 0;
}
