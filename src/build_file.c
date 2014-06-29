#include "build_file.h"

static int write_instruction(struct instr_t in, FILE *file);

int build_file(const char *fname, struct instr_t *ints, size_t len)
{
	unsigned int i;
	FILE *file = fopen(fname, "wb+");
	if (!file){
		return -1;
	}

	for (i = 0; i < len; i++){
		write_instruction(ints[i], file);
	}

	fclose(file);
	return 0;
}

static int write_instruction(struct instr_t in, FILE *file)
{
	fwrite(&in.code, sizeof(uint8_t), 1, file);
	fwrite(&in.arg, sizeof(uint16_t), 1, file);

	return 0;
}