#ifndef __ERRHANDLING_H__
#define __ERRHANDLING_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/*
 * Push a location on the location stack so it will be printed on a call to
 * report_*. A call to this function MUST be paired with a call to 
 * 'pop_location' when leaving the scope.
 *
 * params:
 *	loc - Location to push on the location stack
 *
 * returns:
 *	int - On succes: 0
 *	      On failure: -1
 */
int push_location(const char *loc);

/*
 * Pop the top location off the location stack, so it will no longer be 
 * printed. A call to this function MUST be made after a call to 
 * 'push_location'.
 *
 * returns:
 *	int - On suces: 0
 *	      On failure: -1
 */
int pop_location();

/*
 * Report an error with the code being parsed / compiled, which voilates the
 * rules of the protocol-language, and therefore should result in no output
 * file. The provided error message will be preceded by the current location.
 *
 * params:
 *	fmt - Format string for the error message
 *	... - Arguments for the format string
 *
 * returns:
 *	int - On succes: 0
 *	      On failure: -1
 */
int report_error(const char *fmt, ...);

/*
 * Report a warning which is something that is not an error, but probably not
 * what the user wanted. The provided error message will be preceded by the 
 * current location.
 *
 * params:
 *	fmt - Format string for the error message
 *	... - Arguments for the format string
 *
 * returns:
 *	int - On succes: 0
 *	      On failure: -1
 */
int report_warning(const char *fmt, ...);

/*
 * Report an error which means that the program cannot continue to run (such
 * as providing an input file which does not exits). The provided error 
 * message will be preceded by the current location. NOTE: On calling this
 * function the program will exit immediately with the code 'EXIT_FAILURE'.
 *
 * params:
 *	fmt - Format string for the error message
 *	... - Arguments for the format string 
 */
void report_fatal(const char *fmt, ...);

/*
 * An error has been met: 1
 * No error has been met: 0
 */
extern int met_error;

#endif /* __ERRHANDLING_H__ */