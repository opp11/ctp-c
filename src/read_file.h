#ifndef __READFILE_H__
#define __READFILE_H__

#include <stdio.h>
#include <ctype.h>
#include "instr.h"

struct instr_t *read_file(char *fname);

#endif /* __READFILE_H__ */