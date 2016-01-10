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
	int i;
	int * e;
	list = dll_init();
	i = 0;
	
	for (i = 0; i < 1000000; i++) {
		e = (int *) malloc(sizeof(int));
		*e = i;
		dll_add_back(list, (void *) e);
	}

	dll_foreach(list, my_call_back, NULL);

	dll_destroy(list);
	return 0;
}
