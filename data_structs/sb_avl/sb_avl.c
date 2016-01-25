#include "sb_avl.h"
#include "sb_stack.h"
#include <stdlib.h>

/** User Interface Procedures **/
//*****************************************************************************
sb_avl * avl_init(int (*comp)(void * a, void * b))
{
	sb_avl * instance;
	instance = (sb_avl *) malloc(sizeof(sb_avl));
	instance->cmp_keys = comp;
	instance->root = NULL;
	return instance;
}

void avl_destroy(sb_avl * instance)
{
	// Do an inorder traversal, adding node pointers onto a stack. After the
	// traversal, empty the stack while freeing nodes. Once all nodes are
	// freed, free the stack and free the AVL instance. This ensures that
	// freeing nodes before traversal is complete would not cause the
	// traversal to fail.
	avl_node * node;
	sb_stack * s = stack_init();
	avl_inorder(instance->root, avl_destroy_callback, (void *) s);
	while (!stack_empty(s)) {
		node = (avl_node *) stack_top(s);
		free(node);
		stack_pop(s);
	}
	stack_destroy(s);
	free(instance->root);
	free(instance);
	return;
}

avl_node * avl_root(sb_avl * instance)
{
	return instance->root;
}

void avl_erase(sb_avl * instance, void * key)
{
	avl_node * v = avl_finder(key, avl_root(instance), instance->cmp_keys);
	if ( v == NULL ) {
		return;
	}
	avl_node * w = avl_delete(instance, v);
	avl_rebalance(instance, w);
}

void avl_preorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data)
{
	if (root != NULL)
	{
		call_back(root, user_data);
		avl_preorder(root->left, call_back, user_data);
		avl_preorder(root->right, call_back, user_data);
	}
}

void avl_inorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data)
{
	if (root != NULL)
	{
		avl_inorder(root->left, call_back, user_data);
		call_back(root, user_data);
		avl_inorder(root->right, call_back, user_data);
	}

	return;
}

void avl_postorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data)
{
	if (root != NULL)
	{
		avl_postorder(root->left, call_back, user_data);
		avl_postorder(root->right, call_back, user_data);
		call_back(root, user_data);
	}

	return;
}

void * avl_find(sb_avl * instance, void * key)
{
	avl_node * tmp;
	tmp = avl_finder(key, avl_root(instance), instance->cmp_keys);
	if (tmp == NULL) {
		return NULL;
	}
	else {
		return tmp->value;
	}
}

int avl_size(sb_avl * instance)
{
	if (instance != NULL) {
		return avl_size_helper(instance->root);
	}

	return 0;
}

void avl_insert(sb_avl * instance, void * key, void * data)
{
	avl_node * root = avl_root(instance);
	avl_node * z = avl_new_node(key, data);
	z->parent = NULL;
	avl_node * x = root;
	while (x != NULL) {
		z->parent = x;
		if (instance->cmp_keys(z->key, x->key) < 0) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	if (z->parent == NULL) {
		instance->root = z;
	}
	else if (instance->cmp_keys(z->key, z->parent->key) < 0) {
		z->parent->left = z;
	}
	else {
		z->parent->right = z;
	}

	avl_set_height(z);
	avl_rebalance(instance, z);
	return;
}

avl_node * avl_find_min(sb_avl * instance)
{
	avl_node * x = avl_root(instance);
	while (x->left != NULL) {
		x = x->left;
	}
	return x;
}

avl_node * avl_find_max(sb_avl * instance)
{
	avl_node * x = avl_root(instance);
	while (x->right != NULL) {
		x = x->right;
	}
	return x;
}
//*****************************************************************************

/** Internal Functions: not meant to be called by user **/
//*****************************************************************************

/* Helper function that allocates a new node with the given key and data with
   NULL left, right, and parent pointers. Sets height to -1, designed to mean 
   unspecified. */
avl_node * avl_new_node(void * key, void * data)
{
	avl_node * node = (avl_node *) malloc(sizeof(avl_node));
	node->key = key;
	node->value = data;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->height = 0; // not yet set
	return(node);
}

int avl_height(avl_node * v)
{
	if (v == NULL)
		return 0;
	return (avl_is_external(v)) ? 0 : v->height;
}

void avl_set_height(avl_node * v)
{
	int hl = avl_height(v->left);
	int hr = avl_height(v->right);
	v->height = 1 + avl_max(hl, hr);
}

int avl_max(int a, int b)
{
	if ( a > b )
		return a;
	else
		return b;
}

int avl_is_balanced(avl_node * v)
{
	int bal = avl_height(v->left) - avl_height(v->right);
	return ((-1 <= bal) && (bal <= 1));
}

avl_node * avl_tall_grandchild(avl_node * z)
{
	avl_node * zl = z->left;
	avl_node * zr = z->right;
	if (avl_height(zl) >= avl_height(zr)) {
		if (avl_height(zl->left) >= avl_height(zl->right)) {
			return zl->left;
		}
		else {
			return zl->right;
		}
	}
	else {
		if (avl_height(zr->right) >= avl_height(zr->left)) {
			return zr->right;
		}
		else {
			return zr->left;
		}
	}
}

void avl_expand_external(avl_node * p) 
{
	// already has children
	if (avl_is_internal(p)) {
		return;
	}
	avl_node * v = p;
	v->left = avl_new_node(NULL, NULL);
	v->left->parent = v;
	v->right = avl_new_node(NULL, NULL);
	v->right->parent = v;
	return;
}

int avl_is_internal(avl_node * v)
{
	if (v == NULL)
		return 0;
	return (v->left != NULL || v->right != NULL);
}

int avl_is_external(avl_node * v)
{
	if (v == NULL)
		return 0;
	return (v->left == NULL && v->right == NULL);
}

void avl_destroy_callback(avl_node * n, void * d)
{
	sb_stack * s = (sb_stack *) d;
	stack_push(s, (void *) n);
}

avl_node * avl_finder(void * key, avl_node * node, int (*cmp_keys)(void * a, void * b))
{
	if (node == NULL || cmp_keys(node->key, key) == 0) {
		return node;
	}
	else if (cmp_keys(key, node->key) < 0) {
		return avl_finder(key, node->left, cmp_keys);
	}
	else {
		return avl_finder(key, node->right, cmp_keys);
	}
}

int avl_size_helper(avl_node * root) 
{
	int n = 0;
	if(root != NULL) {
		n = 1 + avl_size_helper(root->left) + avl_size_helper(root->right);
	}
	return n;
}

// Get Balance factor of node N
int avl_get_balance(avl_node * N)
{
	if (N == NULL)
		return 0;
	return avl_height(N->left) - avl_height(N->right);
}

void avl_make_left_child(avl_node * a, avl_node * b)
{
	a->left = b;
	b->parent = a;
}

void avl_make_right_child(avl_node * a, avl_node * b)
{
	a->right = b;
	b->parent = a;
}

avl_node * avl_restructure(sb_avl * instance, avl_node * x)
{
	avl_node * y = x;
	avl_node * z = y->parent;
	avl_node * a, * b, * c;
	avl_node * root;
	root = avl_root(instance);

	if (instance->cmp_keys(z->key, x->key) <= 0 && 
			instance->cmp_keys(x->key, y->key) <= 0) {
		a = z; b = x; c = y;
	}	
	if (instance->cmp_keys(z->key, x->key) >= 0 && 
			instance->cmp_keys(x->key, y->key) >= 0) {
		a = y; b = x; c = z;
	}	
	if (instance->cmp_keys(z->key, y->key) <= 0 && 
			instance->cmp_keys(y->key, x->key) <= 0) {
		a = z; b = y; c = x;
	}	
	if (instance->cmp_keys(z->key, y->key) >= 0 && 
			instance->cmp_keys(y->key, x->key) >= 0) {
		a = x; b = y; c = z;
	}

	if (z == root) {
		instance->root = b;
		b->parent = NULL;
	}
	else {
		if (z->parent->left == z) {
			avl_make_left_child(z->parent, b);
		}
		else {
			avl_make_right_child(z->parent, b);
		}	
	}

	if (b->left != x && b->left != y && b->left != z) {
		avl_make_right_child(a, b->left);
	}

	if (b->right != x && b->right != y && b->right != z) {
		avl_make_left_child(c, b->right);
	}

	avl_make_left_child(b, a);
	avl_make_right_child(b, c);

	return b;
}

void avl_rebalance(sb_avl * instance, avl_node * v)
{
	avl_node * z = v;
	if (v == NULL || v == avl_root(instance)) {
		return;
	}
	while (!(z == avl_root(instance))) {
		z = z->parent;
		avl_set_height(z);
		if (!avl_is_balanced(z)) {
			avl_node * x = avl_tall_grandchild(z);
			z = avl_restructure(instance, x);
			avl_set_height(z->left);
			avl_set_height(z->right);
			avl_set_height(z);
		}
	}
}

avl_node * avl_delete(sb_avl * instance, avl_node * p)
{
	/** Case 1: p has no children **/
	if (p->left == NULL && p->right == NULL) {
		if (p == avl_root(instance)) {
			free(avl_root(instance));
			instance->root = NULL;
			return NULL;
		}

		avl_node * parent = p->parent;

		if (parent->left == p) {
			parent->left = NULL;
		}
		else {
			parent->right = NULL;
		}
		
		free(p);
		return parent;
	}

	/** Case 2: p has 1 child **/
	if (p->right == NULL) {
		if (p == avl_root(instance)) {
			instance->root = instance->root->left;
			free(p);
			return instance->root;
		}

		avl_node * parent = p->parent;

		if (parent->left == p) {
			parent->left = p->left;
		}
		else {
			parent->right = p->left;
		}

		free(p);
		return parent;
	}

	if (p->left == NULL) {
		if ( p == avl_root(instance) ) {
			instance->root = instance->root->right;
			free(p);
			return instance->root;
		}

		avl_node * parent = p->parent;

		if (parent->left == p) {
			parent->left = p->right;
		}
		else {
			parent->right = p->right;
		}

		free(p);
		return parent;
	}
	
	/** Case 3: p has 2 children **/
    if ( p->right->left == NULL ) {

		// overwrite p with p->right.
		// This leaves p->right with no way to
		// get to it. So remove p->right.
		avl_node * tmp = p->right;
    	p->key = p->right->key;
		p->value = p->right->value;
        p->right = p->right->right;

		free(tmp);
		return p->parent;
    }

    avl_node * succ = p->right;
    avl_node * succParent = p;

    while ( succ->left != NULL ) {
    	succParent = succ;
        succ = succ->left;
    }

    p->key = succ->key;
	p->value = succ->value;
    succParent->left = succ->right;
	free(succ);
	return succParent;
}
