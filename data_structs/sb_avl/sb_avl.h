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

sb_avl * avl_init(int (*comp)(void * a, void * b));

void avl_destroy(sb_avl * instance);

avl_node * avl_root(sb_avl * instance);

void avl_erase(sb_avl * instance, void * key);

void avl_preorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data);

void avl_inorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data);

void avl_postorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data);

void * avl_find(sb_avl * instance, void * key);

int avl_size(sb_avl * instance);

void avl_insert(sb_avl * instance, void * key, void * data);

avl_node * avl_find_min(sb_avl * instance);

avl_node * avl_find_max(sb_avl * instance);

avl_node * avl_new_node(void * key, void * data);

int avl_height(avl_node * v);

void avl_set_height(avl_node * v);

int avl_max(int a, int b);

int avl_is_balanced(avl_node * v);

avl_node * avl_tall_grandchild(avl_node * z);

void avl_expand_external(avl_node * p);

int avl_is_internal(avl_node * v);

int avl_is_external(avl_node * v);

void avl_destroy_callback(avl_node * n, void * d);

avl_node * avl_finder(void * key, avl_node * node, int (*cmp_keys)(void * a, void * b));

int avl_size_helper(avl_node * root);

int avl_max(int a, int b);

int avl_get_balance(avl_node * N);

void avl_make_left_child(avl_node * a, avl_node * b);

void avl_make_right_child(avl_node * a, avl_node * b);

avl_node * avl_restructure(sb_avl * instance, avl_node * x);

void avl_rebalance(sb_avl * instance, avl_node * v);

avl_node * avl_delete(sb_avl * instance, avl_node * p);

#endif
