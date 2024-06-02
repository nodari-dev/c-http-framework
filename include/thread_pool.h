#include <pthread.h>

#include "./conf.h"
#include "./request_queue.h"

typedef struct T_Pool {
  pthread_t workers[MAX_T];
} T_Pool;

typedef struct Worker_Args {
  Request_Queue *q;
  pthread_mutex_t *mutex;
  pthread_cond_t *cond;
} Worker_Args;

T_Pool *init_thread_pool(Worker_Args *worker_args);
void shutdown_thread_pool(T_Pool *pool);
