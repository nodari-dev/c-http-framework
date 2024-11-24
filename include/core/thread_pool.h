#include <pthread.h>

#include "../conf.h"
#include "./request_queue.h"
#include "./router.h"

typedef struct TP {
  pthread_t workers[MAX_T];
} TPL;

typedef struct Worker_Args {
  Q *q;
  Router *r;
  pthread_mutex_t *mutex;
  pthread_cond_t *cond;
} Worker_Args;

TPL *init_thread_pool(Worker_Args*);
void shutdown_thread_pool(TPL*);
