#ifndef __READFILE_H__
#define __READFILE_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "err_handling.h"

#define MAX_LINE_DIGITS 512

/*
 * Structure for containing the words in a line.
 *
 * members:
 * 	len   - The number of words for this line.
 *	words - Array of strings containing the words in the line.
 */
struct fline_t {
	int len;
	char **words;
};

/*
 * Reads the contents of a file to be compiled, and returns lines containg
 * instructions as an array of fline_t structs. The return value should be 
 * freed by the user program.
 *
 * params:
 *	fname - Path to the file to be read.
 *	len   - If the function was succesfull, this will contain the number 
 *	        of items in the returned array.
 *
 * returns:
 *	struct fline_t* - Array of fline_t structs containing the 
 *	                  code lines the read file.
 */
struct fline_t *read_file(char *fname, size_t *len);

#endif /* __READFILE_H__ */