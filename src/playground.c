#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/request_queue.h"
#include "../include/thread_pool.h"

void *functionCount1(void *arg);
void *functionCount2(void *arg);
int count = 0;

int main() {

  Request_Queue *request_queue = createQueue();
  pthread_t thread1, thread2, thread3;

  pthread_create(&thread1, NULL, &functionCount1, request_queue);
  pthread_create(&thread3, NULL, &functionCount1, request_queue);
  pthread_create(&thread2, NULL, &functionCount2, request_queue);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  exit(0);
}

void *functionCount1(void *arg) {
  Request_Queue *q = (Request_Queue *)arg;

  for (;;) {
    pthread_mutex_lock(&q->condition_mutex);
    while (q->head == NULL) {
      printf("Worker is waiting\n");
      pthread_cond_wait(&q->not_empty, &q->condition_mutex);
    }
    pthread_mutex_unlock(&q->condition_mutex);

    pthread_mutex_lock(&q->mutex);
    printf("Worker: %d\n", deque(q));
    pthread_mutex_unlock(&q->mutex);
  }
}

void *functionCount2(void *arg) {
  Request_Queue *q = (Request_Queue *)arg;

  for (;;) {
    pthread_mutex_lock(&q->condition_mutex);
    if (q->head != NULL) {
      pthread_cond_signal(&q->not_empty);
    }
    pthread_mutex_unlock(&q->condition_mutex);

    pthread_mutex_lock(&q->mutex);
    count++;
    enque(q, count);
    printf("New request: %d\n", count);
    pthread_mutex_unlock(&q->mutex);

    sleep(5);
  }
}
