#include <stdio.h>
#include "read_file.h"
#include "parse_file.h"
#include "instr.h"
#include "build_file.h"

int main(int argc, char **argv)
{
	size_t len;
	struct fline_t *lines = read_file(argv[1], &len);
	struct instr_t *ints = parse_file(lines, &len);
	build_file("a.prt", ints, len);
	return 0;
}