#ifndef SB_SLL_H
#define SB_SLL_H

typedef struct sb_sll_s {
	struct sb_sll_node_s * head;
	int size;
} sb_sll;

typedef struct sb_sll_node_s {
	void * elem;
	struct sb_sll_node_s * next;
} sb_sll_node;

sb_sll * sll_init();

int sll_size(sb_sll *);
int sll_empty(sb_sll *);

void * sll_front(sb_sll *);

void sll_add_front(sb_sll *, void * elem);

/* removes and frees nodes but does not free elements */
void sll_remove_front(sb_sll *);
void sll_destroy(sb_sll *);

#endif
