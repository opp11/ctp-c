#include "err_handling.h"

struct location_t {
	struct location_t *next;
	struct location_t *prev;
	const char *loc;
};
static struct location_t *root_loc = NULL;
static struct location_t *end_loc = NULL;

int met_error = 0;

static void print_location();

void push_location(const char* loc)
{
	struct location_t *new_loc = malloc(sizeof(struct location_t));
	if (!new_loc){
		clear_location();
		report_fatal("not enough unused RAM");
	}
	new_loc->loc = loc;
	new_loc->next = NULL;
	if (!end_loc){
		/* first push */
		root_loc = new_loc;
		end_loc = new_loc;
		new_loc->prev = NULL;
	} else {
		/* normal push */
		end_loc->next = new_loc;
		new_loc->prev = end_loc;
		end_loc = new_loc;
	}
}

void pop_location()
{
	struct location_t *prev_end = end_loc;
	if (!end_loc){
		/* Emtpy stack so do nothing */
		return;
	} else if (!(end_loc->prev)){
		root_loc = NULL;
		end_loc = NULL;
	} else {
		end_loc = end_loc->prev;
		end_loc->next = NULL;
	}
	free(prev_end);
}

void clear_location()
{
	while (end_loc){
		pop_location();
	}
}

void report_error(const char* fmt, ...)
{
	va_list ap;

	met_error = 1;
	if (end_loc){
		print_location();
	}
	fprintf(stderr, "error: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

void report_warning(const char* fmt, ...)
{
	va_list ap;

	if (end_loc){
		print_location();
	}
	fprintf(stderr, "warning: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

void report_fatal(const char *fmt, ...)
{	
	va_list ap;

	if (end_loc){
		print_location();
	}
	fprintf(stderr, "fatal error: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);

	exit(EXIT_FAILURE);
}

static void print_location()
{
	struct location_t *crnt;
	for (crnt = root_loc; crnt; crnt = crnt->next){
		fprintf(stderr, "%s:", crnt->loc);
	}
	fprintf(stderr, " ");
}