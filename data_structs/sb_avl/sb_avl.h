#ifndef SB_AVL_H
#define SB_AVL_H

// An AVL tree node
typedef struct avl_node_s
{
	void * key;
	void * value;
	struct avl_node_s * left;
	struct avl_node_s * right;
	struct avl_node_s * parent;
	int height;
} avl_node;

// An AVL Tree With Comparison Function Pointer
typedef struct avl_tree {
	avl_node * root;
	int (*cmp_keys)(void * a, void * b);
} sb_avl;

/** User Interface Functions **/
// Create the AVL Tree; initialized with a comparison function
// that returns -1 for a < b, 0 for a = b, and 1 for a > b
sb_avl * avl_init(int (*comp)(void * a, void * b));

// Free the nodes in the AVL tree and the instance itself
void avl_destroy(sb_avl * instance);

// Return the root of the AVL tree
avl_node * avl_root(sb_avl * instance);

// Remove the node with a specific key from the tree
void avl_erase(sb_avl * instance, void * key);

// preorder traversal with a callback function;
// call back function is passed the current node and any data you supply
void avl_preorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data);

// inorder traversal with a callback function
void avl_inorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data);

void avl_postorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data);

// find a value with a specific key in the tree
void * avl_find(sb_avl * instance, void * key);

// same as avl_find but instead of returning a value; 
// return the node itself
avl_node * avl_find_node(sb_avl * instance, void * key);

// how many nodes in the AVL?
int avl_size(sb_avl * instance);

// put an entry into the tree
void avl_insert(sb_avl * instance, void * key, void * data);

// return the minimum node
avl_node * avl_find_min(sb_avl * instance);

// return the maximum node
avl_node * avl_find_max(sb_avl * instance);

/** Internal Book Keeping Functions **/
avl_node * avl_new_node(void * key, void * data);

void avl_destroy_callback(avl_node * n, void * d);

avl_node * avl_finder(void * key, avl_node * node, int (*cmp_keys)(void * a, void * b));

int avl_size_helper(avl_node * root);

void avl_delete(sb_avl * instance, avl_node * p);

avl_node * avl_trinode_restructure(sb_avl * instance, avl_node * x, avl_node * y, avl_node * z);

void avl_rebalance(sb_avl * instance, avl_node * p);

void avl_recomp_height( avl_node * x );

int avl_diff_height(avl_node * t1, avl_node * t2);

int avl_max_height( avl_node * t1, avl_node * t2 );

avl_node * avl_taller_child(avl_node * p);

#endif
