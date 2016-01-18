#ifndef SB_CLL_H
#define SB_CLL_H

typedef struct sb_cl_list_node {
	void * elem;
	struct sb_cl_list_node * next;
} sb_cll_node;

typedef struct sb_cl_list {
	sb_cll_node * cursor;
} sb_cll;

sb_cll * cll_init();

void cll_destroy(sb_cll * instance);

int cll_empty(sb_cll * instance);

void * cll_front(sb_cll * instance);

void * cll_back(sb_cll * instance);

void cll_advance(sb_cll * instance);

void cll_add(sb_cll * instance, void * e);

void cll_remove(sb_cll * instance);

#endif
