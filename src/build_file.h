#ifndef __BUILDFILE_H__
#define __BUILDFILE_H__

#include <stdio.h>
#include "instr.h"

int build_file(const char *fname, struct instr_t *ints, size_t len);

#endif /* __BUILDFILE_H__ */