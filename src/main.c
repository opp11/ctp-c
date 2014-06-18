#include <stdio.h>
#include "read_file.h"
#include "instr.h"

int main(int argc, char **argv)
{
	struct instr_t *ins = read_file("doc/test");
	while ((*ins).code != CODE_END){
		int i = 0;
		printf("%i: ", (*ins).code);
		while (i < (*ins).argc){
			printf("%s ", (*ins).argv[i]);
			i++;
		}
		printf("\n");
		ins++;
	}
	return 0;
}