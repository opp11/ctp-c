#ifndef __BUILDFILE_H__
#define __BUILDFILE_H__

#include <stdio.h>
#include "instr.h"
#include "err_handling.h"

/*
 * Builds a binary file with the name 'fname' containing the provided 
 * instructions. NOTE: if the file already exists it will be overwritten.
 * Furthere the provided instructions must also contain the file header and
 * end code.
 *
 * params:
 * 	fname - The name of the file to build.
 *	ints  - The array of instructions the file will consist of.
 *	len   - The number of items in 'ints'.
 *
 * returns:
 *	int - On success: 0
 *	      On failure: -1
 */
int build_file(const char *fname, struct instr_t *ints, size_t len);

#endif /* __BUILDFILE_H__ */