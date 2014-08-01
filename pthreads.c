
#include "stopif.h"
#include "string_utilities.h"
#include <pthread.h>

typedef struct {
	int wc;
	char *docName;
} wc_struct;

void *wc(void *voidin) {
	wc_struct *in = voidin;
	printf("IN a new thread %i \n",in->wc);

	char *doc = string_from_file(in->docName);
	if (!doc)	
	{
		return NULL;
	}
	char *delimiters = " `!@#$%^&*()_-+={[]}|\\;:\",<>./?\n";
	ok_array *words = ok_array_new(doc, delimiters);
	if (!words)
	{
		return NULL;
	}

	in->wc = words->length;
	ok_array_free(words);

	return NULL;
}


int main(int argc, char **argv) {

	Stopif(!argc, return 0, "Please give some file names on the command line.");
	pthread_t threads[argc];
	wc_struct s[argc];

	
	for (int i = 0; i < argc; i++) {
		s[i] = (wc_struct){.docName=argv[i]};
		pthread_create(&threads[i], NULL, wc, &s[i]);
	}
	
	for (int i = 0; i < argc; i++) {
		pthread_join(threads[i], NULL);
	}

	for (int i = 0; i < argc; ++i)
	{
		printf("%s:\t%i\n", argv[i], s[i].wc);
	}
	return 0;
}

//clang -o pthreads.o  string_utilities.c pthreads.c -I/usr/local/Cellar/glib/2.40.0_1/include/glib-2.0 -I/usr/local/Cellar/glib/2.40.0_1/lib/glib-2.0/include -lglib-2.0 -Os -w -pipe -march=native -mmacosx-version-min=10.10