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
	size_t lines_len;
	struct fline_t *lines;
	struct instr_t *ints;
	FILE *stream;

	process_options(argc, argv);
	if (opt_read_stdin){
		push_location("stdin");
		lines = read_file(stdin, &lines_len);
		if (optind < argc){
			report_warning("ignoring all input files");
		}
	} else {
		if (optind >= argc){
			report_fatal("no input file given");
		}
		push_location(argv[optind]);
		if (optind != argc - 1){
			report_warning("ignoring all other files");
		}
		stream = fopen(argv[optind], "r");
		if (!stream){
			report_fatal("could not open file");
		}
		lines = read_file(stream, &lines_len);
		fclose(stream);
	}
	len = lines_len;
	ints = parse_file(lines, &len);
	free_flines(lines, lines_len);
	if (!met_error && !opt_no_output){
		build_file(opt_outfile, ints, len);
	}
	free(ints);
	pop_location();
	exit(EXIT_SUCCESS);
}