#ifndef __PROGOPTS_H__
#define __PROGOPTS_H__

#include <getopt.h>
#include <stddef.h>
#include "err_handling.h"

extern char *opt_outfile;
extern int opt_no_output;

int process_options(int argc, char **argv);

#endif /* __PROGOPTS_H__ */