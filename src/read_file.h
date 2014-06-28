#ifndef __READFILE_H__
#define __READFILE_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct fline_t {
	int len;
	char **words;
};
extern const struct fline_t END_LINE;

struct fline_t *read_file(char *fname, size_t *len);

#endif /* __READFILE_H__ */