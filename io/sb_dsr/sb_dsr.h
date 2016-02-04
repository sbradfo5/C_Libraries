#ifndef SB_DSR_H
#define SB_DSR_H

typedef struct {
	char * line;
	int lineSize;
} sb_dsr;

sb_dsr * dsr_init();

void dsr_destroy(sb_dsr * instance);

void dsr_expand(sb_dsr * instance);

// reads a line from fn and returns num bytes read
int dsr_getline(sb_dsr * instance, FILE * fh);

#endif
