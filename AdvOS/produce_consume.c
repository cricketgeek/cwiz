#include <stdio.h>
#include "stopif.h"
#include <pthread.h>
#include <stdlib.h>
#include <glib.h>

static GMutex mutex_lock;
int buffer_size;
int buffer_index;

void *producer(void *arg) {
	int *p_buffer = (int*)arg;

	while(buffer_size <= buffer_index) {
		printf("waiting to produce because buffer is full, buffer_index:%d\n", buffer_index);
	}

	g_mutex_lock(&mutex_lock);
	printf("producing...\n");

	int iterations = (buffer_size - buffer_index)/2;
	printf("adding %d items to the buffer\n", iterations);

	for (int i = 0; i < iterations; ++i)
	{
		/* put some stuff in the buffer */
		int data = rand() % 100 + 1;
		/* is this if check necessary */
		if (buffer_index < buffer_size)
		{
			p_buffer[buffer_index++] = data;
		}
	}


	g_mutex_unlock(&mutex_lock);
}

void *consumer(void *arg) {

	int *p_buffer = (int*)arg;

	if (buffer_index > 0)
	{
		g_mutex_lock(&mutex_lock);
		printf("consuming...\n");
		while(buffer_index > 0) {
			/* pull some stuff out of buffer */
			int data = p_buffer[buffer_index--];
			printf("%d pulled out of my buffer\n", data);
		}
		g_mutex_unlock(&mutex_lock);		
	}
}


int main(int argc, char const *argv[])
{
	argc--; //first arg is the program file name

	Stopif((argc != 2), return 0, "Usage 'pthreads 4 100' Please enter number of iterations and buffer capacity.");
	pthread_t pid, cid;

	int *p_buffer;

	int producers_consumers = atoi(argv[1]);
	buffer_size = atoi(argv[2]);

	srand(time(NULL));

	p_buffer = (int*)malloc(sizeof(int)*buffer_size);

	printf("running with %d producers and consumers\n", producers_consumers);

	for (int i = 0; i < producers_consumers; ++i)
	{
		pthread_create(&pid, NULL, producer, (void*)&p_buffer);
		pthread_create(&cid, NULL, consumer, (void*)&p_buffer);
	}

	pthread_join(pid,NULL);
	pthread_join(cid,NULL);

	free(p_buffer);

	return 0;
}