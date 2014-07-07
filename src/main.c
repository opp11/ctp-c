#include <stdio.h>
#include <getopt.h>
#include "read_file.h"
#include "parse_file.h"
#include "instr.h"
#include "build_file.h"
#include "err_handling.h"
#include "prog_opts.h"

int main(int argc, char **argv)
{
	size_t len;
	process_options(argc, argv);
	if (optind >= argc){
		report_fatal("no input file given");
	}
	push_location(argv[optind]);
	struct fline_t *lines = read_file(argv[optind], &len);
	struct instr_t *ints = parse_file(lines, &len);
	if (!met_error && !opt_no_output){
		build_file(opt_outfile, ints, len);
	}
	pop_location();
	exit(EXIT_SUCCESS);
}