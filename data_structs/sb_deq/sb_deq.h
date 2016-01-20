#ifndef SB_DEQ_H
#define SB_DEQ_H

typedef struct sb_dll_s sb_dll;

typedef struct {
	sb_dll * D;
	int size;
} sb_deq;

sb_deq * deq_init();

void deq_destroy();

int deq_size(sb_deq * instance);

int deq_empty(sb_deq * instance);

void * deq_front(sb_deq * instance);

void * deq_back(sb_deq * instance);

void deq_insert_front(sb_deq * instance, void * e);

void deq_insert_back(sb_deq * instance, void * e);

void deq_remove_front(sb_deq * instance);

void deq_remove_back(sb_deq * instance);

#endif
