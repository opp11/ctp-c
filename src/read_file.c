#include "read_file.h"

static const char COMMENT_CHAR = '#';

static struct instr_t get_instruction(char *line);
static int is_code_line(char *line);

struct instr_t *read_file(char *fname)
{
	struct instr_t *out = NULL;
	size_t out_len = 0;
	char *line = NULL;
	size_t line_len = 0;

	FILE *file = fopen(fname, "r");
	if (!file){
		return NULL;
	}

	while (getline(&line, &line_len, file) != -1){
		if (is_code_line(line)){
			out_len++;
			out = realloc(out, sizeof(struct instr_t) * out_len);
			out[out_len - 1] = get_instruction(line);
		}
	}

	out_len++;
	out = realloc(out, sizeof(struct instr_t) * out_len);
	out[out_len - 1] = END_INSTR;

	fclose(file);
	return out;
}

static struct instr_t get_instruction(char *line)
{
	struct instr_t out = {0, NULL, 0};
	char *crnt_word = strtok(line, " ");
	out.code = instr_name_to_code(crnt_word);

	while ((crnt_word = strtok(NULL, " \n"))){
		out.argc++;
		out.argv = realloc(out.argv, sizeof(char*) * out.argc);
		out.argv[out.argc - 1] = 
			malloc(sizeof(char) * strlen(crnt_word) + 1);
		strcpy(out.argv[out.argc - 1], crnt_word);
	}

	return out;
}

static int is_code_line(char *line)
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