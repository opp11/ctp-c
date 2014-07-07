#include "read_file.h"

static const char COMMENT_CHAR = '#';
static const char *INSTR_SEPS = " \n";

/* Splits a line in the file into words */
static struct fline_t split_line(char *line);

/* Checks whether a line is considered a code line or a skippable */
static int is_code_line(char *line);

/* Convert a null-terminated string to lowercase */
static void str_to_lower(char* str);

struct fline_t *read_file(FILE *stream, size_t *len)
{
	struct fline_t *out = NULL;
	size_t out_len = 0;
	char *line = NULL;
	size_t line_len = 0;

	while (getline(&line, &line_len, stream) != -1){
		out_len++;
		out = realloc(out, sizeof(struct fline_t) * out_len);
		if (is_code_line(line)){
			str_to_lower(line);
			out[out_len - 1] = split_line(line);
		} else {
			out[out_len - 1].len = 0;
			out[out_len - 1].words = NULL;
		}
	}
	free(line);

	if (len){
		(*len) = out_len;
	}
	return out;
}

static struct fline_t split_line(char *line)
{
	struct fline_t out = {0, NULL};
	char *crnt_word = strtok(line, INSTR_SEPS);

	while (crnt_word){
		out.len++;
		out.words = realloc(out.words, sizeof(char*) * out.len);
		out.words[out.len - 1] = 
			malloc(sizeof(char) * strlen(crnt_word) + 1);
		strcpy(out.words[out.len - 1], crnt_word);
		crnt_word = strtok(NULL, INSTR_SEPS);
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

static void str_to_lower(char* str)
{
	while (*str){
		(*str) = tolower(*str);
		str++;
	}
}