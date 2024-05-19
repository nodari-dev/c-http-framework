#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "request_queue.h"
#include "conf.h"

typedef struct Thread_Pool{
	int active_threads;
	// replace with furser configuration
	pthread_t threads[MAX_T];
	Request_Queue* request_queue;
	int shutdown;
} Thread_Pool;

Thread_Pool* init_thread_pool(Request_Queue* request_queue);

