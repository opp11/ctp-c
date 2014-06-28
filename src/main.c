#include <stdio.h>
#include "read_file.h"
#include "parse_file.h"
#include "instr.h"

int main(int argc, char **argv)
{
	struct fline_t *lines = read_file("doc/test");
	struct instr_t *ints = parse_file(lines);
	while (ints->code != CODE_END){
		printf("%i:%i\n", ints->code, ints->arg);
		ints++;
	}
	return 0;
}