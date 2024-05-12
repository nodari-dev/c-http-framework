#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/conf.h"
#include "../include/errors.h"
#include "../include/http_parser.h"
#include "../include/request_queue.h"

#include "../include/conf.h"

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

  printf("Server is running on host: %s\n", DEFAULT_HOST);
  int counter = 1;

  struct Queue *request_queue = createQueue();

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

    char* buffer = (char*)malloc(BUFFER_FOR_READ * sizeof(char));
    if (buffer == NULL) {
        perror("malloc");
        return FAILED;
    }
	int read_result = recv(client_socket_fd, buffer, BUFFER_FOR_READ, 0);
    if (read_result == FAILED) {
      perror("Failed reading message\n");
    }

    HTTP_REQUEST* http_request = read_http_request(buffer);
    if (http_request) {
      printf("%d %s %s\n", http_request->method, http_request->uri,
             http_request->version);
      struct HTTP_HEADER *header = http_request->headers;
      // while (header) {
      //   printf("%s: %s\n", header->name, header->value);
      //   header = header->next;
      // }
      // if (http_request->body) {
      //   printf("BODY: %s\n", http_request->body);
      // }
    } else {
      printf("%s\n", "parsing error");
    }

    enque(request_queue, counter);

	//  AFTER
	free_http_request(http_request);
	free(buffer);
	//  AFTER
	//
    char response[2048];
    sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 26\r\n"
            "\r\n"
            "<h1>%d</h1>",
            counter);

    counter++;

    int write_res = write(client_socket_fd, response, sizeof(response));
    if (write_res == FAILED) {
      printf("Message was not sent\n");
    }

    close(client_socket_fd);
  }

  close(server_socket_fd);

  return 0;
}
