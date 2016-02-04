#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sb_dsr.h"

sb_dsr * dsr_init()
{
	sb_dsr * instance;
	instance = (sb_dsr *) malloc(sizeof(sb_dsr));
	instance->line = (char *) calloc(1024, sizeof(char));
	instance->lineSize = 1024;
	instance->line[0] = '\0';
	return instance;
}

void dsr_destroy(sb_dsr * instance)
{
  free(instance->line);
  free(instance);
  return;
}

void dsr_expand(sb_dsr * instance)
{
	char * newBuf;

  // alloc double the memory
  newBuf = (char *) calloc(instance->lineSize * 2, sizeof(char));
  newBuf[instance->lineSize * 2 - 1] = '\0';

  // copy the contents of line to newBuf
  newBuf = memcpy(newBuf, instance->line, instance->lineSize);

  // free old buffer contents and make line point to newBuf
  free(instance->line);
  instance->line = newBuf;
  instance->lineSize = instance->lineSize * 2;
}

int dsr_getline(sb_dsr * instance, FILE * fh)
{
	int c;
	int bytesRead;

	bytesRead = 0;
	instance->line[0] = '\0';
	c = fgetc(fh);

	if (c == EOF) {
		return -1;
	}
	while (c != EOF && c != (int) '\n') {
		if (bytesRead + 1 >= instance->lineSize - 1) {
      dsr_expand(instance);
		}

		// add character to the line and null terminate
		instance->line[bytesRead] = c;
		instance->line[bytesRead + 1] = '\0';
		bytesRead++;
		c = fgetc(fh);
	}

	return bytesRead;
}
