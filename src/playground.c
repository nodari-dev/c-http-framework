#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/request_queue.h"
#include "../include/thread_pool.h"
#include "../include/conf.h"
#include "../include/errors.h"
#include "../include/http_parser.h"
#include "../include/request_reader.h"
#include "../include/request_queue.h"
#include "../include/request_reader.h"

void *worker_thread(void *arg);
void *request_thread(void *arg);

int main() {

  Request_Queue *request_queue = createQueue();
  pthread_t thread1, thread2, thread3;

  pthread_create(&thread1, NULL, &worker_thread, request_queue);
  pthread_create(&thread3, NULL, &worker_thread, request_queue);
  pthread_create(&thread2, NULL, &request_thread, request_queue);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  return 0;
}

void *worker_thread(void *arg) {
  Request_Queue *q = (Request_Queue *)arg;

  for (;;) {

    pthread_mutex_lock(&q->condition_mutex);
    while (q->head == NULL) {
      printf("Worker is waiting\n");
      pthread_cond_wait(&q->not_empty, &q->condition_mutex);
    }
    pthread_mutex_unlock(&q->condition_mutex);

    pthread_mutex_lock(&q->mutex);
	int client_socket_fd = deque(q);
    printf("Worker: %d\n", client_socket_fd);


    char *buffer = read_request(client_socket_fd);
    if (buffer == NULL) {
      close(client_socket_fd);
    }

    HTTP_REQUEST *http_request = parse_http_request(buffer);
    if (http_request != NULL) {
      printf("%sThread is working on s\n", http_request->uri);
    } else {
      printf("%s\n", "parsing error");
    }

    //  AFTER
    free(buffer);
    buffer = NULL;

    char response[2048];
    sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 26\r\n"
            "\r\n"
            "<h1>you suck</h1>");


    int write_res = write(client_socket_fd, response, sizeof(response));
    if (write_res == -1) {
      printf("Message was not sent\n");
    }

    free_http_request(http_request);
    http_request = NULL;
    close(client_socket_fd);
    pthread_mutex_unlock(&q->mutex);
  }
}

void *request_thread(void *arg) {
  struct sockaddr_in host_address;
  host_address.sin_family = AF_INET;
  host_address.sin_port = htons(PORT);
  host_address.sin_addr.s_addr = inet_addr(DEFAULT_HOST);

  int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket_fd == -1) {
    perror("Couldn't create a socket\n");
  }

  if (bind(server_socket_fd, (struct sockaddr *)&host_address,
           sizeof(host_address)) == -1) {
    perror("Couldn't bind a socket\n");
  }

  if (listen(server_socket_fd, SOMAXCONN) == -1) {
    perror("this shit ain't gonna listen\n");
  }

  Request_Queue *q = (Request_Queue *)arg;

  for (;;) {
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(host_address);

    int client_socket_fd =
        accept(server_socket_fd, (struct sockaddr *)&client_address,
               &client_address_len);

    if (client_socket_fd == -1) {
      perror("Failed connecting to client\n");
      continue;
    }

    pthread_mutex_lock(&q->mutex);
    enque(q, client_socket_fd);
    printf("New request: %d\n", client_socket_fd);
    pthread_mutex_unlock(&q->mutex);

    pthread_mutex_lock(&q->condition_mutex);
    if (q->head != NULL) {
		printf("SIGNAL MOTHER FUCKER\n");
      pthread_cond_signal(&q->not_empty);
    }
    pthread_mutex_unlock(&q->condition_mutex);
  }

  close(server_socket_fd);
}
