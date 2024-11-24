#include <pthread.h>

#ifndef _RQ
#define _RQ

typedef struct QN {
  struct QN *next;
  int client_socket_fd;
} QN;

typedef struct Q{
	int len;
	struct QN *head;
	struct QN *tail;
} Q;

Q* createQueue();
void enque(Q*, int);
int deque(Q*);
void monitor_queue(Q*);
void free_request_queue(Q*);

#endif
