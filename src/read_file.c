#include "read_file.h"

static const char COMMENT_CHAR = '#';

static struct instr_t get_instruction(char *line);
static int is_code_line(char* line);

struct instr_t *read_file(char *fname)
{
	struct instr_t *out = NULL;
	char *line = NULL;
	size_t line_len = 0;

	FILE *file = fopen(fname, "r");
	if (!file){
		return NULL;
	}

	while (getline(&line, &line_len, file) >= 0){
		if (is_code_line(line)){
			printf("%s", line);
		}
	}
	printf("\n");

	fclose(file);
	return out;
}

static struct instr_t get_instruction(char *line)
{
	struct instr_t out;
	return out;
}

static int is_code_line(char* line)
{
	if (line && (*line) == COMMENT_CHAR){
		return 0;
	}
	while (*line){
		if (isalnum(*line)){
			return 1;
		}
		line++;
	}
	return 0;
}