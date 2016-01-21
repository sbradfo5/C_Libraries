#include "sb_avl.h"
#include "sb_stack.h"
#include <stdlib.h>

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

void avl_insert(sb_avl * instance, void * key, void * data)
{
	instance->root = avl_insert_node(instance->root, key, data, instance->cmp_keys);
}

void avl_erase(sb_avl * instance, void * key)
{
	instance->root = avl_delete_node(instance->root, key, instance->cmp_keys);
}

void * avl_find(sb_avl * instance, void * key)
{
	avl_node * tmp;
	tmp = avl_find_node(instance->root, key, instance->cmp_keys);
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

void avl_preorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data)
{
	if(root != NULL)
	{
		call_back(root, user_data);
		avl_preorder(root->left, call_back, user_data);
		avl_preorder(root->right, call_back, user_data);
	}
}

void avl_inorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data)
{
	if(root != NULL)
	{
		avl_inorder(root->left, call_back, user_data);
		call_back(root, user_data);
		avl_inorder(root->right, call_back, user_data);
	}
}

void avl_postorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data)
{
	if(root != NULL)
	{
		avl_postorder(root->left, call_back, user_data);
		avl_postorder(root->right, call_back, user_data);
		call_back(root, user_data);
	}
}

/* 
   Everything below this point is 'private'. You should not be calling these
   unless you are very comfortable with the structure of the AVL tree
*/

void avl_destroy_callback(avl_node * n, void * d)
{
	sb_stack * s = (sb_stack *) d;
	stack_push(s, (void *) n);
}

avl_node * avl_find_node(avl_node * node, void * key, int (*cmp_keys)(void * a, void * b))
{
	if (node == NULL || cmp_keys(node->key, key) == 0) {
		return node;
	}
	else if (cmp_keys(key, node->key) < 0) {
		return avl_find_node(node->left, key, cmp_keys);
	}
	else {
		return avl_find_node(node->right, key, cmp_keys);
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

// A utility function to get height of the tree
int avl_height(avl_node * N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

// A utility function to get maximum of two integers
int avl_max(int a, int b)
{
	return (a > b)? a : b;
}

/* Helper function that allocates a new node with the given key and
   NULL left and right pointers. */
avl_node * avl_new_node(void * key, void * data)
{
	avl_node * node = (avl_node *) malloc(sizeof(avl_node));
	node->key = key;
	node->value = data;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;  // new node is initially added at leaf
	return(node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
avl_node * avl_right_rotate(avl_node * y)
{
	avl_node * x = y->left;
	avl_node * T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = avl_max(avl_height(y->left), avl_height(y->right))+1;
	x->height = avl_max(avl_height(x->left), avl_height(x->right))+1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
avl_node * avl_left_rotate(avl_node * x)
{
	avl_node * y = x->right;
	avl_node * T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	//  Update heights
	x->height = avl_max(avl_height(x->left), avl_height(x->right))+1;
	y->height = avl_max(avl_height(y->left), avl_height(y->right))+1;

	// Return new root
	return y;
}

// Get Balance factor of node N
int avl_get_balance(avl_node * N)
{
	if (N == NULL)
		return 0;
	return avl_height(N->left) - avl_height(N->right);
}

avl_node * avl_insert_node(avl_node * node, void * key, void * data, int (*cmp_keys)(void * a, void * b))
{
	/* 1. Perform the normal BST rotation */
	if (node == NULL)
		return avl_new_node(key, data);

	if (cmp_keys(key, node->key) < 0)
		node->left  = avl_insert_node(node->left, key, data, cmp_keys);
	else
		node->right = avl_insert_node(node->right, key, data, cmp_keys);

	/* 2. Update height of this ancestor node */
	node->height = avl_max(avl_height(node->left), avl_height(node->right)) + 1;

	/* 3. Get the balance factor of this ancestor node to check whether
	   this node became unbalanced */
	int balance = avl_get_balance(node);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && cmp_keys(key, node->left->key) < 0)
		return avl_right_rotate(node);

	// Right Right Case
	if (balance < -1 && cmp_keys(key, node->right->key) > 0)
		return avl_left_rotate(node);

	// Left Right Case
	if (balance > 1 && cmp_keys(key, node->left->key) > 0)
	{
		node->left =  avl_left_rotate(node->left);
		return avl_right_rotate(node);
	}

	// Right Left Case
	if (balance < -1 && cmp_keys(key, node->right->key) < 0)
	{
		node->right = avl_right_rotate(node->right);
		return avl_left_rotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

/* Given a non-empty binary search tree, return the node with minimum
   key value found in that tree. Note that the entire tree does not
   need to be searched. */
avl_node * avl_min_value_node(avl_node * node)
{
	avl_node * current = node;

	/* loop down to find the leftmost leaf */
	while (current->left != NULL)
		current = current->left;

	return current;
}

avl_node * avl_delete_node(avl_node * root, void * key, int (*cmp_keys)(void * a, void * b))
{
	// STEP 1: PERFORM STANDARD BST DELETE

	if (root == NULL)
		return root;

	// If the key to be deleted is smaller than the root's key,
	// then it lies in left subtree
	if ( cmp_keys(key, root->key) < 0 )
		root->left = avl_delete_node(root->left, key, cmp_keys);

	// If the key to be deleted is greater than the root's key,
	// then it lies in right subtree
	else if( cmp_keys(key, root->key) > 0 )
		root->right = avl_delete_node(root->right, key, cmp_keys);

	// if key is same as root's key, then this is the node
	// to be deleted
	else
	{
		// node with only one child or no child
		if( (root->left == NULL) || (root->right == NULL) )
		{
			avl_node * temp = root->left ? root->left : root->right;

			// No child case
			if(temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case
				*root = *temp; // Copy the contents of the non-empty child

			free(temp);
		}
		else
		{
			// node with two children: Get the inorder successor (smallest
			// in the right subtree)
			avl_node * temp = avl_min_value_node(root->right);

			// Copy the inorder successor's data to this node
			root->key = temp->key;

			// Delete the inorder successor
			root->right = avl_delete_node(root->right, temp->key, cmp_keys);
		}
	}

	// If the tree had only one node then return
	if (root == NULL)
		return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
	root->height = avl_max(avl_height(root->left), avl_height(root->right)) + 1;

	// STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether
	//  this node became unbalanced)
	int balance = avl_get_balance(root);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && avl_get_balance(root->left) >= 0)
		return avl_right_rotate(root);

	// Left Right Case
	if (balance > 1 && avl_get_balance(root->left) < 0)
	{
		root->left = avl_left_rotate(root->left);
		return avl_right_rotate(root);
	}

	// Right Right Case
	if (balance < -1 && avl_get_balance(root->right) <= 0)
		return avl_left_rotate(root);

	// Right Left Case
	if (balance < -1 && avl_get_balance(root->right) > 0)
	{
		root->right = avl_right_rotate(root->right);
		return avl_left_rotate(root);
	}

	return root;
}
