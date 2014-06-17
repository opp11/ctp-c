#include "read_file.h"

static struct instr_t read_instruction(FILE *file);
static char *read_line(FILE *file);
static int is_code_line(char* line);

struct instr_t *read_file(char *fname)
{
	struct instr_t *out;
	FILE *file = fopen(fname, "r");
	if (!file){
		return NULL;
	}

	printf("%s\n", read_line(file));

exit:
	fclose(file);
	return out;
}

static struct instr_t read_instruction(FILE *file)
{

}

static char *read_line(FILE *file)
{
	int crnt_size = 30;
	char *out = malloc(crnt_size * sizeof(char));
	char crnt = 0;
	int i = 0;
	while ((crnt = fgetc(file)) != '\n' && crnt != EOF){
		out[i] = crnt;
		i++;
		if (i >= crnt_size){
			crnt_size += 10;
			out = realloc(out, crnt_size);
		}
		out[i] = 0;
	}
	return out;
}

static int is_code_line(char* line)
{

}