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
	avl_node * n;

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
	printf("inserted Alfon.\n");

	string = strdup("Bekah");
	val = (int *) malloc(sizeof(int));
	*val = 150;
	avl_insert(tree, (void *) string, (void *) val);
	printf("inserted Bekah.\n");

	string = strdup("Debra");
	val = (int *) malloc(sizeof(int));
	*val = 45;
	avl_insert(tree, (void *) string, (void *) val);
	printf("inserted Debra.\n");

	coreyStr = strdup("Corey");
	coreyVal = (int *) malloc(sizeof(int));
	*coreyVal = 80;
	avl_insert(tree, (void *) coreyStr, (void *) coreyVal);
	printf("inserted Corey.\n");

	printf("AVL size: %d\n", avl_size(tree));

	// let's go through them
	avl_inorder(tree->root, print_elements, NULL);

	// Now, let's find a couple of them
	val = (int *) avl_find(tree, "Alfon");
	printf("Alfon's score: %d\n", *val);

	val = (int *) avl_find(tree, "Corey");
	printf("Corey's score: %d\n", *val);

	// let's search for something that isn't there
	val = (int *) avl_find(tree, "Zach");
	if (val == NULL) {
		printf("Zach Not found. Success.\n");
	}
	else {
		printf("Zach was found? Error.\n");
	}

	// Alright; time to delete a few
	n = avl_find_node(tree, "Alfon");
	string = (char *) n->key;
	val = (int *) n->value;
	avl_erase(tree, "Alfon");
	free(string);
	free(val);

	n = avl_find_node(tree, "Debra");
	string = (char *) n->key;
	val = (int *) n->value;
	avl_erase(tree, "Debra");
	free(string);
	free(val);

	// let's go through them now that two are gone.
	avl_preorder(tree->root, print_elements, NULL);

	// Let's free the rest of the memory
	avl_postorder(tree->root, free_elements, NULL);

	// and destroy the AVL
	avl_destroy(tree);

	return EXIT_SUCCESS;
}
