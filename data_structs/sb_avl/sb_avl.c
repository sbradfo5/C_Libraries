#include "sb_avl.h"
#include "sb_stack.h"
#include <stdlib.h>
#include <stdio.h>

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
	avl_delete(instance, v);
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

avl_node * avl_find_node(sb_avl * instance, void * key)
{
	avl_node * tmp;
	tmp = avl_finder(key, avl_root(instance), instance->cmp_keys);
	if (tmp == NULL) {
		return NULL;
	}
	else {
		return tmp;
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
	avl_node * curr_node, * prev_node, * q, * x, * y, * z;

	// insert into empty AVL tree
	if (avl_root(instance) == NULL) {
		instance->root = avl_new_node(key, data);
		instance->root->height = 1;
		return;
	}

	curr_node = instance->root;  
	prev_node = instance->root;

	// traverse until NULL, keeping track of the parent
	while ( curr_node != NULL )
	{
		if ( instance->cmp_keys(key, curr_node->key ) < 0 )
		{
			prev_node = curr_node;       // Remember prev. node
			curr_node = curr_node->left;  // Continue search in left subtree
		}
		else if ( instance->cmp_keys(key, curr_node->key ) > 0 )
		{
			prev_node = curr_node;       // Remember prev. node
			curr_node = curr_node->right; // Continue search in right subtree
		}
	}

	q = avl_new_node(key, data);
	q->height = 1;
	q->parent = prev_node;

	if ( instance->cmp_keys(key, prev_node->key) < 0 ) {
		prev_node->left = q;
	}
	else {
		prev_node->right = q;
	}

	x = y = z = q;

	while ( x != NULL ) {
		if ( avl_diff_height(x->left, x->right) <= 1) {
			z = y;
			y = x;
			x = x->parent;
		}
		else {
			break;
		}
	}

	if ( x != NULL ) {
		avl_trinode_restructure(instance, x, y, z);
	}
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

avl_node * avl_trinode_restructure(sb_avl * instance, avl_node * x, avl_node * y, avl_node * z)
{
	avl_node * a, * b, * c;
	avl_node * T0, * T1, * T2, * T3;
	avl_node * xParent;

	int zIsLeftChild = (z == y->left);
	int yIsLeftChild = (y == x->left);

	if (zIsLeftChild && yIsLeftChild) {
		a = z;
		b = y;
		c = x;
		T0 = z->left;
		T1 = z->right;
		T2 = y->right;
		T3 = x->right;
	}
	else if (!zIsLeftChild && yIsLeftChild) {
		a = y;
		b = z;
		c = x;
		T0 = y->left;
		T1 = z->left;
		T2 = z->right;
		T3 = x->right;
	}
	else if (zIsLeftChild && !yIsLeftChild) {
		a = x;
		b = z;
		c = y;
		T0 = x->left;
		T1 = z->left;
		T2 = z->right;
		T3 = y->right;
	}
	else {
		a = x;
		b = y;
		c = z;
		T0 = x->left;
		T1 = y->left;
		T2 = z->left;
		T3 = z->right;
	}

	if ( x == instance->root ) {
		instance->root = b;
		b->parent = NULL;
	}
	else {
		xParent = x->parent;

		if ( x == xParent->left ) {
			b->parent = xParent;
			xParent->left = b;
		}
		else {
			b->parent = xParent;
			xParent->right = b;
		}
	}

	b->left = a;
	a->parent = b;
	b->right = c;
	c->parent = b;

	a->left = T0;
	if ( T0 != NULL ) {
		T0->parent = a;
	}

	a->right = T1;
	if ( T1 != NULL ) {
		T1->parent = a;
	}

	c->left = T2;
	if ( T2 != NULL ) {
		T2->parent = c;
	}

	c->right = T3;
	if ( T3 != NULL ) {
		T3->parent = c;
	}

	avl_recomp_height(a);
	avl_recomp_height(c);

	return b;
}

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

void avl_delete(sb_avl * instance, avl_node * p)
{
	/** Case 1: p has no children **/
	if (p->left == NULL && p->right == NULL) {
		/** Case 1a: p is the root **/
		if (p == avl_root(instance)) {
			free(avl_root(instance));
			return;
		}
		
		avl_node * parent = p->parent;

		if (parent->left == p) {
			parent->left = NULL;
		}
		else {
			parent->right = NULL;
		}

		avl_recomp_height(parent);

		avl_rebalance(instance, parent);

		free(p);

		return;
	}

	/** Case 2: p has 1 child **/
	if (p->right == NULL) {

		/** Case 2a: p is the root **/
		if (p == avl_root(instance)) {

			// update the new root's parent
			instance->root->left->parent = NULL;

			// change the root to the new root
			instance->root = instance->root->left;

			// free the old root
			free(p);

			// re-compute heights
			avl_recomp_height(instance->root);

			// and re-balance
			avl_rebalance(instance, instance->root);
			return;
		}

		avl_node * parent = p->parent;

		if (parent->left == p) {
			parent->left = p->left;
			p->left->parent = parent;
		}
		else {
			parent->right = p->left;
			p->left->parent = parent;
		}

		free(p);

		avl_recomp_height(parent);

		avl_rebalance(instance, parent);

		return;
	}

	if (p->left == NULL) {
		if ( p == avl_root(instance) ) {
			// update the new root's parent
			instance->root->right->parent = NULL;

			// change the root
			instance->root = instance->root->right;

			// free the old root
			free(p);

			// re-compute heights
			avl_recomp_height(instance->root);

			// and re-balance
			avl_rebalance(instance, instance->root);

			return;
		}

		avl_node * parent = p->parent;

		if (parent->left == p) {
			parent->left = p->right;
			p->right->parent = parent;
		}
		else {
			parent->right = p->right;
			p->right->parent = parent;
		}

		free(p);

		avl_recomp_height(parent);

		avl_rebalance(instance, parent);

		return;
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

		avl_recomp_height(p->parent);

		avl_rebalance(instance, p->parent);

		return;
	}

	avl_node * succ = p->right;
	avl_node * succParent;

	while ( succ->left != NULL ) {
		succ = succ->left;
	}

	p->key = succ->key;
	p->value = succ->value;

	succParent = succ->parent;
	succParent->left = succ->right;

	free(succ);

	avl_recomp_height(succParent);
	
	avl_rebalance(instance, succParent);

	return;
}

void avl_recomp_height( avl_node * x )
{
	while ( x != NULL )
	{
		x->height = avl_max_height( x->left, x->right ) + 1;
		x = x->parent;
	}
}

int avl_diff_height(avl_node * t1, avl_node * t2)
{
	int h1, h2;

	if ( t1 == NULL )
		h1 = 0;
	else
		h1 = t1->height;

	if ( t2 == NULL )
		h2 = 0;
	else
		h2 = t2->height;

	return ((h1 >= h2) ? (h1-h2) : (h2-h1)) ;
}

int avl_max_height( avl_node * t1, avl_node * t2 )
{
	int h1, h2;

	if ( t1 == NULL )
		h1 = 0;
	else
		h1 = t1->height;

	if ( t2 == NULL )
		h2 = 0;
	else
		h2 = t2->height;

	return (h1 >= h2) ? h1 : h2 ;
}

void avl_rebalance(sb_avl * instance, avl_node * p)
{
	avl_node * x, * y, * z;

	while ( p != NULL )
	{
		if ( avl_diff_height(p->left, p->right) > 1 )
		{
			x = p;
			y = avl_taller_child(x);
			z = avl_taller_child(y);

			p = avl_trinode_restructure(instance, x, y, z);
		}

		p = p->parent;
	}
}

avl_node * avl_taller_child(avl_node * p)
{
	if ( p->left == NULL )
		return p->right;

	if ( p->right == NULL )
		return p->left;

	if ( p->left->height > p->right->height )
		return p->left;
	else
		return p->right;
}
