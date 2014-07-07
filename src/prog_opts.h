#ifndef __PROGOPTS_H__
#define __PROGOPTS_H__

#include <getopt.h>
#include <stddef.h>
#include "err_handling.h"

/* Contains the name of the output file. Is './out.prt' as default. */
extern char *opt_outfile;
/* Specifies if an output file should be created. 0 if yes, 1 if no. */
extern int opt_no_output;

/*
 * Process all the commandline options and stores them in their relevant
 * global variables. NOTE: if an unknown option is met, or an option is
 * missing an argument it is considered a fatal error, and reported as such.
 *
 * params:
 *	argc - The commandline argument count as passed to main.
 *	argv - The commandline arguments as passed to main.
 */
void process_options(int argc, char **argv);

#endif /* __PROGOPTS_H__ */