#include <stdio.h>
#include "read_file.h"
#include "instr.h"

int main(int argc, char **argv)
{
	struct fline_t *lines = read_file("doc/test");
	while ((*lines).len != -1){
		int i = 0;
		while (i < (*lines).len){
			printf("%s | ", (*lines).words[i]);
			i++;
		}
		printf("\n");
		lines++;
	}
	return 0;
}