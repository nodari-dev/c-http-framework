#include <pthread.h>

#ifndef _REQUESTQUEUEH 
#define _REQUESTQUEUEH

typedef struct Request_Queue{
	int len;
	struct QNode *head;
	struct QNode *tail;
} Request_Queue;

Request_Queue* createQueue();
void enque(Request_Queue *queue, int client_socket_fd);
int deque(Request_Queue *queue);
void monitor_queue(Request_Queue *queue);
#endif
