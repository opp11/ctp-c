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
	FILE *stream = NULL;
	process_options(argc, argv);
	if (optind >= argc){
		report_fatal("no input file given");
	}
	push_location(argv[optind]);
	stream = fopen(argv[optind], "r");
	if (!stream){
		report_fatal("could not open file");
	}
	struct fline_t *lines = read_file(stream, &len);
	fclose(stream);
	struct instr_t *ints = parse_file(lines, &len);
	if (!met_error && !opt_no_output){
		build_file(opt_outfile, ints, len);
	}
	pop_location();
	exit(EXIT_SUCCESS);
}