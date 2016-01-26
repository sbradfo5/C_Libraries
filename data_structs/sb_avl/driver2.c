#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sb_avl.h"

void remove_newlines(char * str)
{
	char * newline; 
	while ( (newline = strchr(str, '\n')) != NULL ) {
		*newline = '\0';
	}
	return;
}

int comp_strs(void * a, void * b)
{
	return strcmp((char *) a, (char *) b);
}

void find_wrong_parent(avl_node * n, void * data)
{
	if (n->parent) {
		if (n->parent->parent == n) {
			printf("FOUND WRONG PARENT: %s %d\n", (char *) n->key, *((int *) n->value));
		}
	}
}
void print_elements(avl_node * n, void * data)
{
	int * num;
	num = (int *) n->value;
	printf("key: %s, value: %d\n", (char *) n->key, *num);
}

void find_and_erase(sb_avl * tree, char * key)
{
	int * value;
	char * str;
	avl_node * n = avl_find_node(tree, (void *) key);
	value = (int *) n->value;
	str = (char *) n->key; 
	avl_erase(tree, (void *) key);
	free(str);
	free(value);
}

void free_elements(avl_node * n, void * data)
{
	int * num;
	char * key;
	key = (char *) n->key;
	num = (int *) n->value;
	free(key);
	free(num);
}

void read_into_avl(sb_avl * tree, FILE * f)
{
	char buf[1000];
	char * name;
	char * N;
	int * number;

	while (fgets(buf, 1000, f)) {
		// remove the newline
		remove_newlines(buf);

		// split the fields
		name = strdup(strtok(buf, " "));
		
		N = strtok(NULL, " ");
		number = (int *) malloc(sizeof(int));

		sscanf(N, "%d", number);

		// store the key-value pairs
		avl_insert(tree, (void *) name, (void *) number);
	}
}

int main(int argc, char ** argv)
{
	FILE * f;
	sb_avl * tree;
	int * value;

	// create the AVL tree
	tree = avl_init(comp_strs);

	// read all the boy name statistics into the AVl. Each line will have the
	// form "name score"
	f = fopen("boyNames.txt", "r");
	read_into_avl(tree, f);
	fclose(f);

	// read girl name statistics into the AVL.
	f = fopen("girlNames.txt", "r");
	read_into_avl(tree, f);
	fclose(f);

	// traverse it in order, printing each entry of the AVL
	avl_inorder(tree->root, print_elements, NULL);

	// print the number of names
	printf("\nAVL Size: %d\n\n", avl_size(tree));

	// find an element
	value = avl_find(tree, "Sebastian");
	
	printf("Sebastian's value: %d\n\n", *value);
	
	avl_inorder(tree->root, find_wrong_parent, NULL);

	find_and_erase(tree, "Sebastian");
	find_and_erase(tree, "Brody");
	find_and_erase(tree, "Jaxson");
	find_and_erase(tree, "George");
	find_and_erase(tree, "Eli");
	find_and_erase(tree, "Aaliyah");
	find_and_erase(tree, "Savannah");
	find_and_erase(tree, "Zoey");
	find_and_erase(tree, "Zoe");
	find_and_erase(tree, "Victoria");

	avl_inorder(tree->root, find_wrong_parent, NULL);

	printf("AVL Size after 10 deletions: %d\n\n", avl_size(tree));

	avl_inorder(tree->root, print_elements, NULL);

	// clean up memory
	avl_inorder(tree->root, free_elements, NULL);
	avl_destroy(tree);

	return EXIT_SUCCESS;
}
