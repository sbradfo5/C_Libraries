#ifndef SB_QUEUE_H
#define SB_QUEUE_H

typedef struct sb_cl_list sb_cll;

typedef struct {
	sb_cll * C;
	int size;
} sb_queue;

sb_queue * queue_init();

void queue_destroy(sb_queue * instance);

int queue_size(sb_queue * instance);

int queue_empty(sb_queue * instance);

void * queue_front(sb_queue * instance);

void queue_enqueue(sb_queue * instance, void * e);

void queue_dequeue(sb_queue * instance);

#endif
