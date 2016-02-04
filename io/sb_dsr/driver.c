#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sb_dsr.h"

int main(int argc, char ** argv)
{
	sb_dsr * reader;
	FILE * fh;

	fh = fopen("test.txt", "r+");

	reader = dsr_init();

	while (dsr_getline(reader, fh) != -1) {
		printf("%d: %d %s\n", reader->lineSize, (int) strlen(reader->line), reader->line);
	}

	return EXIT_SUCCESS;
}
