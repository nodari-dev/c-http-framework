#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/conf.h"
#include "../include/errors.h"
#include "../include/http_parser.h"
#include "../include/logger.h"
#include "../include/request_queue.h"
#include "../include/request_reader.h"
#include "../include/thread_pool.h"
#include "../include/utils/string_builder.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int main() {
  struct sockaddr_in host_address;
  host_address.sin_family = AF_INET;
  host_address.sin_port = htons(PORT);
  host_address.sin_addr.s_addr = inet_addr(DEFAULT_HOST);

  int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket_fd == FAILED) {
    perror("Couldn't create a socket\n");
    return 1;
  }
  if (bind(server_socket_fd, (struct sockaddr *)&host_address,
           sizeof(host_address)) == FAILED) {
    perror("Couldn't bind a socket\n");
    return 1;
  }

  if (listen(server_socket_fd, SOMAXCONN) == FAILED) {
    perror("this shit ain't gonna listen\n");
    return 1;
  }

  Request_Queue *request_queue = createQueue();
  Worker_Args *worker_args = (Worker_Args *)malloc(sizeof(Worker_Args));
  if (worker_args == NULL) {
    perror("Worker_Args malloc");
    exit(1);
  }

  worker_args->cond = &cond;
  worker_args->mutex = &mutex;
  worker_args->q = request_queue;
  T_Pool *t_pool = init_thread_pool(worker_args);

  String_Builder *sb = init_string_builder();
  if (sb == NULL) {
    perror("String_Builder was not initialized");
    exit(1);
  }
  append_chars(sb, "Server started, listening on port: ");
  char port_str[5];
  sprintf(port_str, "%d", PORT);
  append_chars(sb, port_str);
  log_data(INFO, sb->buf);
  free_string_builder(sb);

  while (1) {
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(host_address);

    int client_socket_fd =
        accept(server_socket_fd, (struct sockaddr *)&client_address,
               &client_address_len);

    if (client_socket_fd == FAILED) {
      log_data(WARNING, "Failed connecting to client");
      continue;
    }

    pthread_mutex_lock(&mutex);
    enque(request_queue, client_socket_fd);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }

  free_string_builder(sb);
  close(server_socket_fd);

  return 0;
}
