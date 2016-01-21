#ifndef SB_AVL_H
#define SB_AVL_H

// An AVL tree node
typedef struct avl_node_s
{
	void * key;
	void * value;
	struct avl_node_s * left;
	struct avl_node_s * right;
	int height;
} avl_node;

// An AVL Tree instance: this is an abstraction that makes it easier on the
// programmer. It's a cleaner interface.
typedef struct avl_tree {
	avl_node * root;
	int (*cmp_keys)(void * a, void * b);
} sb_avl;

sb_avl * avl_init(int (*comp)(void * a, void * b));

void avl_destroy(sb_avl * instance);

void avl_insert(sb_avl * instance, void * key, void * data);

void avl_erase(sb_avl * instance, void * key);

void * avl_find(sb_avl * instance, void * key);

int avl_size(sb_avl * instance);

void avl_preorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data);

void avl_inorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data);

void avl_postorder(avl_node * root, void (*call_back)(avl_node * n, void * d), void * user_data);

void avl_destroy_callback(avl_node * n, void * d);

avl_node * avl_find_node(avl_node * node, void * key, int (*cmp_keys)(void * a, void * b));

int avl_size_helper(avl_node * root);

int avl_max(int a, int b);

int avl_height(avl_node * N);

avl_node * avl_new_node(void * key, void * data);

avl_node * avl_right_rotate(avl_node * y);

avl_node * avl_left_rotate(avl_node * x);

int avl_get_balance(avl_node * N);

avl_node * avl_insert_node(avl_node * node, void * key, void * data, int (*cmp_keys)(void * a, void * b));

avl_node * avl_min_value_node(avl_node * node);

avl_node * avl_delete_node(avl_node * root, void * key, int (*cmp_keys)(void * a, void * b));

#endif
