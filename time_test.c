#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	time_t biggest = 0x7FFFFFFF;

	printf("%s\n", asctime(gmtime(&biggest)) );

	return 0;
}