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

	buffer_index++; //move to empty spot
	if (buffer_index < buffer_size)
	{
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
		buffer_index--; //move back to last position filled

		printf("buffer_index is %d after production\n", buffer_index);
		g_mutex_unlock(&mutex_lock);
	}
}

void *consumer(void *arg) {

	int *p_buffer = (int*)arg;

	g_mutex_lock(&mutex_lock);
	if (buffer_index > 0)
	{
		printf("consuming...\n");
		while(buffer_index > 0) {
			/* pull some stuff out of buffer */
			int data = p_buffer[buffer_index];

			if ((data < 0) || (data > 100))
			{
				printf("pulled a weird value out of buffer %d\n", data);
				printf("buffer index is at %d\n",buffer_index);
			}
			else
			{
				printf("%d pulled out of my buffer\n", data);
			}

			if (buffer_index > 0) buffer_index--;
		}
	}
	printf("buffer_index is %d after consumption\n", buffer_index);
	g_mutex_unlock(&mutex_lock);
}


int main(int argc, char const *argv[])
{
	argc--; //first arg is the program file name

	Stopif((argc != 2), return 0, "Usage 'pthreads 4 100' Please enter number of iterations and buffer capacity.");
	pthread_t pid, cid;

	int *p_buffer;

	int producers_consumers = atoi(argv[1]);
	buffer_size = atoi(argv[2]);
	buffer_index = 0;

	printf("running with %d producers and consumers buffer_size:%d\n", producers_consumers,buffer_size);

	srand(time(NULL));

	p_buffer = (int*)malloc(sizeof(int)*buffer_size);

	for (int i = 0; i < producers_consumers; ++i)
	{
		pthread_create(&pid, NULL, producer, (void*)&p_buffer);
		pthread_create(&cid, NULL, consumer, (void*)&p_buffer);
	}

	pthread_join(pid,NULL);
	pthread_join(cid,NULL);

	printf("buffer_index at %d buffer has %d items\n", buffer_index, (sizeof(p_buffer)/sizeof(p_buffer[0])));
	free(p_buffer);

	return 0;
}