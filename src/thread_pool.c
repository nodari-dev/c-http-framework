#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/conf.h"
#include "../include/request_queue.h"
#include "../include/thread_pool.h"

void *working_thread(void *arg) {
  Thread_Pool *thread_pool = (Thread_Pool *)arg;
  while (1) {
    // pthread_mutex_lock(&thread_pool->request_queue->mutex);

    while (thread_pool->request_queue->head == NULL && !thread_pool->shutdown) {
      pthread_cond_wait(&thread_pool->request_queue->not_empty,
                        &thread_pool->request_queue->mutex);
    }

    printf("lets get to work\n");

    if (thread_pool->shutdown) {
      pthread_mutex_unlock(&thread_pool->request_queue->mutex);
      pthread_exit(NULL);
    }

    int client_socket_id = deque(thread_pool->request_queue);
    // pthread_mutex_unlock(&thread_pool->request_queue->mutex);
    printf("%d\n", client_socket_id);

    printf("i did something\n");
  }

  return NULL;
}

Thread_Pool *init_thread_pool(Request_Queue *request_queue) {
  Thread_Pool *thread_pool = (Thread_Pool *)malloc(sizeof(Thread_Pool));
  thread_pool->shutdown = 0;
  if (thread_pool == NULL) {
    perror("therad pool malloc");
    exit(1);
  }

  thread_pool->request_queue = request_queue;

  for (int i = 0; i < MAX_T; ++i) {
    pthread_create(&thread_pool->threads[i], NULL, working_thread,
                   (void *)thread_pool);
  }

  pthread_exit(NULL);

  return thread_pool;
}

void gracefuly_shutdown_thread_pool() {}
