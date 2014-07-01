#include <stdio.h>
#include "read_file.h"
#include "parse_file.h"
#include "instr.h"
#include "build_file.h"
#include "err_handling.h"

static const char *strip_path(const char* str);

int main(int argc, char **argv)
{
	size_t len;
	push_location(argv[1]);
	struct fline_t *lines = read_file(argv[1], &len);
	struct instr_t *ints = parse_file(lines, &len);
	if (!met_error){
		build_file("a.prt", ints, len);
	}
	pop_location();
	return 0;
}

static const char *strip_path(const char* str)
{
	char *out = strrchr(str, '/');
	return out ? ++out : str;
}