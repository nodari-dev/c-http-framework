#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
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

  char buffer[BUFFER_FOR_READ] =
      "PUT /index.html HTTP/1.1\r\n"
      "Host: www.example.com\r\n"
      "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
      "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 "
      "Safari/537.36\r\n"
      "Accept: "
      "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/"
      "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
      "Accept-Language: en-US,en;q=0.9\r\n"
      "Content-Length: 30\r\n"
      "Connection: keep-alive\r\n"
      "\r\n"
      "This is the request body, if any.";

  HTTP_REQUEST *http_request = read_http_request(buffer);
  if (http_request) {
    printf("%d %s %s\n", http_request->method, http_request->uri,
           http_request->version);
    struct HTTP_HEADER *header = http_request->headers;
    while (header) {
      printf("%s: %s\n", header->name, header->value);
      header = header->next;
    }
    printf("%s\n", http_request->body);
  } else {
    printf("%s\n", "parsing error");
  }
  //
  // while (1) {
  //
  //   struct sockaddr_in client_address;
  //   socklen_t client_address_len = sizeof(host_address);
  //
  //   int client_socket_fd =
  //       accept(server_socket_fd, (struct sockaddr *)&client_address,
  //              &client_address_len);
  //
  //   if (client_socket_fd == FAILED) {
  //     perror("Failed connecting to client\n");
  //     continue;
  //   }
  //
  //   char buffer[BUFFER_FOR_READ];
  //   int read_result = read(client_socket_fd, &buffer, BUFFER_FOR_READ);
  //   if (read_result == FAILED) {
  //     perror("Failed reading message\n");
  //   }
  //
  //   enque(request_queue, counter);
  //
  //   char response[2048];
  //   sprintf(response,
  //           "HTTP/1.1 200 OK\r\n"
  //           "Content-Type: text/html\r\n"
  //           "Content-Length: 26\r\n"
  //           "\r\n"
  //           "<h1>%d</h1>",
  //           counter);
  //
  //   counter++;
  //
  //   int write_res = write(client_socket_fd, response, sizeof(response));
  //   if (write_res == FAILED) {
  //     printf("Message was not sent\n");
  //   }
  //
  //   close(client_socket_fd);
  // }

  close(server_socket_fd);

  return 0;
}
