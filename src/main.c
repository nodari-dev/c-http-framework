#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/conf.h"
#include "../include/errors.h"
#include "../include/http.h"
#include "../include/queue.h"

#include "../include/conf.h"

typedef struct {
  char method[16];
  char path[256];
  char http_version[16];
} HttpRequest;

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
    char buffer[BUFFER_FOR_READ] = "POST /coffee HTTP/1.1\r\n"
                                "Host: 127.0.0.1:8080\r\n"
                                "User-Agent: curl/7.81.0\r\n"
                                "Accept: */*\r\n"
                                "Content-Type: application/json\r\n"
                                "Content-Length: 17\r\n"
                                "\r\n"
                                "{       \"key\": \"value\"}";
  	read_http_request(buffer);
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
  //   char *buffer[BUFFER_FOR_READ];
  //   int read_result = read(client_socket_fd, &buffer, BUFFER_FOR_READ);
  //   if (read_result == FAILED) {
  //     perror("Failed reading message\n");
  //   }
  //
  // 	read_http_request(buffer);
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
