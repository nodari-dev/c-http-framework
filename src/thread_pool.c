#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/conf.h"
#include "../include/http_parser.h"
#include "../include/request_queue.h"
#include "../include/request_reader.h"
#include "../include/thread_pool.h"

void *worker_thread(void *args);

T_Pool *init_thread_pool(Worker_Args *worker_args) {
  T_Pool *pool = (T_Pool *)malloc(sizeof(T_Pool));
  if (pool == NULL) {
    perror("T_Pool malloc");
    exit(1);
  }

  for (int i = 0; i < MAX_T; ++i) {
    if (pthread_create(&pool->workers[i], NULL, worker_thread, worker_args) !=
        0) {
      perror("Thread creating failure\n");
      exit(1);
    }
  }

  return pool;
}

void shutdown_thread_pool(T_Pool *pool) {}

void *worker_thread(void *args) {
  Worker_Args *worker_args = (Worker_Args *)args;
  Request_Queue *request_queue = worker_args->q;
  pthread_mutex_t *mutex = worker_args->mutex;
  pthread_cond_t *cond = worker_args->cond;

  while (1) {
    int client_socket_fd;
    pthread_mutex_lock(mutex);
    if ((client_socket_fd = deque(request_queue)) == -1) {
      pthread_cond_wait(cond, mutex);
    }
    pthread_mutex_unlock(mutex);

    if (client_socket_fd != -1) {
      char *buffer = read_request(client_socket_fd);

      HTTP_REQUEST *http_request = parse_http_request(buffer);
      if (http_request) {
        printf("%d %s %s\n", http_request->method, http_request->uri,
               http_request->version);
      } else {
        printf("Couldn't parse request\n");
      }

      free(buffer);
      buffer = NULL;
      free_http_request(http_request);

      char response[2048];
      sprintf(response, "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: 26\r\n"
                        "\r\n"
                        "<h1>you suck</h1>");

      int write_res = write(client_socket_fd, response, sizeof(response));
      if (write_res == -1) {
        printf("Message was not sent\n");
      }

      close(client_socket_fd);
    }
  }
  return NULL;
}
