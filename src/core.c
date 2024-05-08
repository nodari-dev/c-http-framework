#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/errors.h"
#include "../include/queue.h"
#include "../include/conf.h"


int create_server_socket(struct sockaddr_in *address) {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) {
    printf("Couldn't create a socket'\n");
    return -1;
  }
  printf("Created a socket\n");

  if (bind(socketfd, (struct sockaddr *)address, sizeof(*address)) != 0) {
    printf("Couldn't bind a socket\n");
    return -1;
  }

  printf("Server socket has been binded\n");

  if (listen(socketfd, SOMAXCONN) != 0) {
    printf("this shit ain't gonna listen\n");
    return -1;
  }

  return socketfd;
}

void start_server(struct sockaddr_in *host_address){
  int server_socket_fd = create_server_socket(host_address);
  if (server_socket_fd == ERROR_DURRING_SOCKET_CREATION) {
    perror("Server start failed\n");
  }

  printf("Server is running on host: %s\n", DEFAULT_HOST);
  int counter = 1;

  struct Queue *request_queue = createQueue();

  while (1) {

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(*host_address);

    int client_socket_fd =
        accept(server_socket_fd, (struct sockaddr *)&client_address,
               &client_address_len);

    if (client_socket_fd == -1) {
      continue;
    }

    char buffer[BUFFER_FOR_READ];

    int read_result = read(client_socket_fd, &buffer, BUFFER_FOR_READ);

    if (read_result == -1) {
      perror("Failed reading message\n");
    }

    enque(request_queue, counter);

    monitor_queue(request_queue);

    char response[2048];
    sprintf(response,
            "HTTP/1.0 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 26\r\n"
            "\r\n"
            "<h1>%d</h1>",
            counter);

    counter++;

    write(client_socket_fd, response, sizeof(response));

    close(client_socket_fd);
  }

  close(server_socket_fd);
}
