#ifndef __PARSEFILE_H__
#define __PARSEFILE_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "instr.h"
#include "read_file.h"

struct instr_t *parse_file(struct fline_t *lines, size_t *len);

#endif /* __PARSEFILE_H__ */