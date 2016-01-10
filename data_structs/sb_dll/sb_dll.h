#ifndef SB_DLL_H
#define SB_DLL_H

typedef struct sb_dll_node_s {
	void * elem;
	struct sb_dll_node_s * next;
	struct sb_dll_node_s * prev;
} sb_dll_node;

typedef struct sb_dll_s {
	sb_dll_node * header;
	sb_dll_node * trailer;
} sb_dll;

sb_dll * dll_init();

void dll_destroy(sb_dll * instance);

int dll_empty(sb_dll * instance);

void * dll_front(sb_dll * instance);

void * dll_back(sb_dll * instance);

void dll_add_front(sb_dll * instance, void * e);

void dll_add_back(sb_dll * instance, void * e);

void dll_remove_front(sb_dll * instance);

void dll_remove_back(sb_dll * instance);

void dll_add_before(sb_dll_node * v, void * e);

void dll_remove(sb_dll_node * v);

void dll_foreach(sb_dll * instance, void (*call_back)(void * d, void * e), void * data);

/* Iterator code for traversing without knowing structure. */
sb_dll_node * dll_begin(sb_dll * instance);

sb_dll_node * dll_end(sb_dll * instance);

// Moves v forward one node. 
int dll_next_node(sb_dll_node * v);

// Moves v back one node.
int dll_prev_node(sb_dll_node * v);

void * dll_get_node_val(sb_dll_node * v);

#endif
