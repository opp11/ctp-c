#include "prog_opts.h"

char *opt_outfile = "./a.prt";
int opt_no_output = 0;
int opt_read_stdin = 0;

static const char *help_msg = "\
Program for compiling a protocol file to a binary file which can be uploaded\n\
to the component tester's SD card.\n\
\n\
  usage:\n\
     ctp [-h | --help] [-o FILE | --outfile=FILE] [-t | --test]\n\
         [INFILE | -r | --read]\n\
\n\
  -h --help                 show this and then exit\n\
  -o FILE --outfile=FILE    specify output file [default: ./out.prt]\n\
  -r --read                 read code directly from stdin\n\
  -t --test                 do compilation but make no output file";

static const char *short_opts = ":o:rth";
static const struct option long_opts[] = {
	{ .name="outfile", .has_arg=required_argument, .flag=NULL, .val='o' },
	{ .name="test", .has_arg=no_argument, .flag=NULL, .val='t' },
	{ .name="read", .has_arg=no_argument, .flag=NULL, .val='r' },
	{ .name="help", .has_arg=no_argument, .flag=NULL, .val='h' },
	{ .name=0, .has_arg=0, .flag=NULL, .val=0}
};

void process_options(int argc, char **argv)
{
	int opt;
	int i;
	opterr = 0; /* make sure getopt's error messages are turned off */
	while ((opt = getopt_long(argc, argv, 
			short_opts, long_opts, &i)) != -1){
		switch (opt){
		case 'o': /* outfile */
			opt_outfile = optarg;
			break;
		case 't': /* test */
			opt_no_output = 1;
			break;
		case 'r': /* read */
			opt_read_stdin = 1;
			break;
		case 'h': /* help */
			printf("%s\n", help_msg);
			exit(EXIT_SUCCESS);
		case '?':
			clear_location();
			report_fatal("unknown option '%s' use -h or --help to get a list of options",
				argv[optind - 1]);
		case ':':
			clear_location();
			report_fatal("option '%s' needs an argument", 
				argv[optind - 1]);
		}
	}
}