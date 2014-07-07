#include "build_file.h"

static void write_instruction(struct instr_t in, FILE *file);

void build_file(const char *fname, struct instr_t *ints, size_t len)
{
	unsigned int i;
	FILE *file = fopen(fname, "wb+");
	if (!file){
		report_fatal("could not create file '%s'", fname);
	}

	write_instruction(HEADER_INSTR, file);
	for (i = 0; i < len; i++){
		write_instruction(ints[i], file);
	}
	write_instruction(END_INSTR, file);

	fclose(file);
}

static void write_instruction(struct instr_t in, FILE *file)
{
	fwrite(&in.code, sizeof(uint8_t), 1, file);
	fwrite(&in.arg, sizeof(uint16_t), 1, file);
}