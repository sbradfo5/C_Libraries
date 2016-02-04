#include <stdio.h>
#include <stdlib.h>
#include "sb_dsr.h"

sb_dsr * dsr_init()
{
	sb_dsr * instance;
	instance = (sb_dsr *) malloc(sizeof(sb_dsr));
	instance->line = (char *) calloc(128, 1);
	instance->lineSize = 128;
	instance->line[0] = '\0';
	return instance;
}

int dsr_getline(sb_dsr * instance, FILE * fh)
{
	int c, i;
	int bytesRead;
	char * newBuf;

	bytesRead = 0;
	instance->line[0] = '\0';
	c = fgetc(fh);

	if (c == EOF) {
		return -1;
	}
	while (c != EOF && c != (int) '\n') {
		if (bytesRead + 1 >= instance->lineSize - 1) {
			// alloc double the memory
			newBuf = (char *) calloc(instance->lineSize * 2, 1);
			newBuf[instance->lineSize * 2 - 1] = '\0';

			// copy contents
			for (i = 0; i < instance->lineSize; i++) {
				newBuf[i] = instance->line[i];
			}
	
			// free old buffer and make line point to new memory
			free(instance->line);
			instance->line = newBuf;
			instance->lineSize = instance->lineSize * 2;
		}

		// add character to the line
		instance->line[bytesRead] = c;
		instance->line[bytesRead + 1] = '\0';
		bytesRead++;
		c = fgetc(fh);
	}

	return bytesRead;
}
