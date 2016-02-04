#include <stdio.h>
#include <stdlib.h>
#include "sb_dsr.h"

int main(int argc, char ** argv)
{
	sb_dsr * reader;

	reader = dsr_init();

	while (dsr_getline(reader, stdin) != -1) {
		printf("%s\n", reader->line);
	}
  
  dsr_destroy(reader);

	return EXIT_SUCCESS;
}
