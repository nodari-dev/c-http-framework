#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../../include/conf.h"
#include "../../include/utils/string_builder.h"
#include "../../include/logger.h"
#include "../../include/http/http.h"
#include "../../include/core/core.h"

void *worker_thread(void *args);

TPL *init_thread_pool(Worker_Args *args) {
  TPL *pool = (TPL *)malloc(sizeof(TPL));
  if (pool == NULL) {
    perror("T_Pool malloc");
    exit(1);
  }

  for (int i = 0; i < MAX_T; ++i) {
    if (pthread_create(&pool->workers[i], NULL, worker_thread, args) !=
        0) {
      perror("Thread creating failure\n");
      exit(1);
    }
  }

  return pool;
}

void shutdown_thread_pool(TPL *pool) {}

void *worker_thread(void *args) {
  Worker_Args *worker_args = (Worker_Args *)args;
  Q *request_queue = worker_args->q;
  Router *router = worker_args->r;

  pthread_mutex_t *mutex = worker_args->mutex;
  pthread_cond_t *cond = worker_args->cond;
  String_Builder *sb = init_string_builder();

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
        append_chars(sb, http_method_to_str(http_request->method));
        append_chars(sb, "");
        append_chars(sb, http_request->uri);
        append_chars(sb, "");
        append_chars(sb, http_request->version);
        log_data(INFO, sb->buf);
        free_string_builder(sb);
      } else {
        log_data(ERROR, "Couldn't parse request");

		// TODO: ADD 500
      }


	  char* response = call_endpoint(router, http_request);
	  printf("%s\n", response);
      int write_res = write(client_socket_fd, response, sizeof(response));
      if (write_res == -1) {
        log_data(ERROR, "Message was not sent\n");
      }
      close(client_socket_fd);
      free(buffer);
      free_http_request(http_request);
    }
  }
  return NULL;
}
