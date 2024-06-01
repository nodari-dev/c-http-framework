#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/conf.h"
#include "../include/errors.h"
#include "../include/http_parser.h"
#include "../include/request_queue.h"
#include "../include/request_reader.h"

#include "../include/conf.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *worker_thread(void *args) {
  Request_Queue *request_queue = (Request_Queue *)args;
  while (1) {
    int client_socket_fd;

    pthread_mutex_lock(&mutex);
    if ((client_socket_fd = deque(request_queue)) == -1){
      pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    // if(client_socket_fd != NULL){
    //
    // }

    char *buffer = read_request(client_socket_fd);

    HTTP_REQUEST *http_request = parse_http_request(buffer);
    if (http_request) {
      printf("%d %s %s\n", http_request->method, http_request->uri,
             http_request->version);
    } else {
      printf("%s\n", "parsing error");
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
    if (write_res == FAILED) {
      printf("Message was not sent\n");
    }

    close(client_socket_fd);
  }
  return NULL;
}

int main() {
  pthread_t pool[10];

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

  printf("Server is running on host: %s\n", DEFAULT_HOST);

  Request_Queue *request_queue = createQueue();

  for (int i = 0; i < 10; ++i) {
    pthread_create(&pool[i], NULL, worker_thread, request_queue);
  }

  while (1) {
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(host_address);

    int client_socket_fd =
        accept(server_socket_fd, (struct sockaddr *)&client_address,
               &client_address_len);

    if (client_socket_fd == FAILED) {
      perror("Failed connecting to client\n");
      continue;
    }

    pthread_mutex_lock(&mutex);
    enque(request_queue, client_socket_fd);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }

  close(server_socket_fd);

  return 0;
}
