#ifndef __PARSEFILE_H__
#define __PARSEFILE_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "instr.h"
#include "read_file.h"

/*
 * Parses the instructions in 'lines', converts them to their binary
 * representations (compiles them), and stores the result in an array of 
 * instr_t structs.
 * In case if any errors with the instruction in lines, the funtion will still
 * return an array, print an error message, and ḿet_err will be set to 1.
 * The return value should ALWAYS be freed by the user program.
 * 
 * params:
 * 	lines - The instructions to parse and compile.
 *	len   - The number if items in 'lines'. If the function was 
 *	        succesfull, this will contain the number of items in the
 *	        returned array.
 *
 * returns:
 *	struct instr_t* - Array of instr_t structs which contain the binary
 *	                  representation of the instructions in 'lines'.
 */
struct instr_t *parse_file(struct fline_t *lines, size_t *len);

#endif /* __PARSEFILE_H__ */