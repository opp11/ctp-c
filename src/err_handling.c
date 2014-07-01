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

int push_location(const char* loc)
{
	struct location_t *new_loc = malloc(sizeof(struct location_t));
	if (!new_loc){
		return -1;
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
	return 0;
}

int pop_location()
{
	struct location_t *prev_end = end_loc;
	end_loc = end_loc->prev;
	if (!end_loc){
		root_loc = NULL;
	} else {
		end_loc->next = NULL;
	}
	free(prev_end);
	return 0;
}

int report_error(const char* fmt, ...)
{
	va_list ap;

	met_error = 1;
	if (!end_loc){
		return -1;
	}

	print_location();
	fprintf(stderr, " error: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);

	return 0;
}

int report_warning(const char* fmt, ...)
{
	va_list ap;

	if (!end_loc){
		return -1;
	}

	fprintf(stderr, "warning: ");
	print_location();
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);

	return 0;
}

void report_fatal(const char *fmt, ...)
{	
	va_list ap;

	if (end_loc){
		fprintf(stderr, "error: ");
		print_location();
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
		va_end(ap);
	}

	exit(EXIT_FAILURE);
}

static void print_location()
{
	struct location_t *crnt;
	for (crnt = root_loc; crnt; crnt = crnt->next){
		fprintf(stderr, "%s:", crnt->loc);
	}
}