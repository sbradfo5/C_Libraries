#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sb_avl.h"

int cmp_strings(void * a, void * b)
{
	return strcmp((char *) a, (char *) b);
}

void print_elements(avl_node * n, void * myData)
{
	int * e;
	e = (int *) n->value;
	printf("key: %s element: %d\n", (char *) n->key, *e);
}

void free_elements(avl_node * n, void * myData)
{
	int * e;
	char * s;
	e = (int *) n->value;
	s = (char *) n->key;
	free(e);
	free(s);
}

int main(int argc, char ** argv)
{
	char * string, * coreyStr;
	int * val, * coreyVal;

	// create the AVL tree
	sb_avl * tree;
	tree = avl_init(cmp_strings);

	// let's add some elements: for the sake of this, these are scores
	// This could be made more general with a large file of name-score
	// pairs
	string = strdup("Alfon");
	val = (int *) malloc(sizeof(int));
	*val = 100;
	avl_insert(tree, (void *) string, (void *) val);

	string = strdup("Bekah");
	val = (int *) malloc(sizeof(int));
	*val = 150;
	avl_insert(tree, (void *) string, (void *) val);

	string = strdup("Debra");
	val = (int *) malloc(sizeof(int));
	*val = 45;
	avl_insert(tree, (void *) string, (void *) val);

	coreyStr = strdup("Corey");
	coreyVal = (int *) malloc(sizeof(int));
	*coreyVal = 80;
	avl_insert(tree, (void *) coreyStr, (void *) coreyVal);

	printf("AVL size: %d\n", avl_size(tree));

	// let's go through them
	avl_inorder(tree->root, print_elements, NULL);

	// Now, let's find a couple of them
	val = (int *) avl_find(tree, "Alfon");
	printf("Alfon's score: %d\n", *val);

	val = (int *) avl_find(tree, "Debra");
	printf("Debra's score: %d\n", *val);

	// let's search for something that isn't there
	val = (int *) avl_find(tree, "Zach");
	if (val == NULL) {
		printf("Zach Not found. Success.\n");
	}
	else {
		printf("Zach was found? Error.\n");
	}

	// let's delete someone
	avl_erase(tree, "Corey");
	printf("Corey deleted.\n");

	// and then search for them
	val = (int *) avl_find(tree, "Corey");
	if (val == NULL) {
		printf("Corey Not found. Success.\n");
	}
	else {
		printf("Corey was found? Error.\n");
	}

	// Free Corey's memory, since we deleted his node earlier
	free(coreyStr);
	free(coreyVal);

	// let's go through them now that one is gone.
	avl_preorder(tree->root, print_elements, NULL);

	// Let's free the rest of the memory
	avl_postorder(tree->root, free_elements, NULL);

	// and destroy the AVL
	avl_destroy(tree);

	return EXIT_SUCCESS;
}
