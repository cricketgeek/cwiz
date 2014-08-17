#include <stdio.h>
#include <stdlib.h> //abort

/** set this to \c 'c' to stop the program on an error
	otherwise, functions return a value on failure */
char error_mode;

FILE *error_log;

#define Stopif(assertion, error_action, ...)					\
	if (assertion) {											\
		fprintf(error_log ? error_log : stderr, __VA_ARGS__);	\
		fprintf(error_log ? error_log : stderr, "\n");			\
		if (error_mode=='s') abort();							\
		else				   {error_action;}					\
	}									