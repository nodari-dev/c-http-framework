#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "errors.h"
#include "socketgenerator.h"

#define PORT 8080
#define BUFFER_FOR_READ 2048

int main() {

  struct sockaddr_in host_address;
  host_address.sin_family = AF_INET;
  host_address.sin_port = htons(PORT);
  host_address.sin_addr.s_addr = inet_addr("127.0.0.1");

  int socketfd = generate_socket(&host_address);

  if (socketfd == ERROR_DURRING_SOCKET_CREATION) {
    printf("Server start failed\n");
    return 1;
  }

  printf("Server is working on host: ...\n");
  int counter = 0;
  while (1) {

    // setting variables for client socket data
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(host_address);

    int client_socket_fd = accept(socketfd, (struct sockaddr *)&client_address,
                                  &client_address_len);

    if (client_socket_fd < 0) {
      perror("Couldn't establish connection with client'\n");
      continue;
    }

    // 	printf("Established connection with client\n");
    //
    char buffer[BUFFER_FOR_READ];

    int read_result = read(client_socket_fd, &buffer, BUFFER_FOR_READ);

    if (read_result == -1) {
      perror("Message failed\n");
    }

    // char method[BUFFER_FOR_READ];
    // char uri[BUFFER_FOR_READ];
    // char version[BUFFER_FOR_READ];
    // sscanf(buffer, "%s %s %s", method, uri, version);
    // printf("[%s:%u] %s %s %s\n", inet_ntoa(client_address.sin_addr),
    // ntohs(client_address.sin_port), method, version, uri);

    counter++;
    printf("%d\n", counter);

    char response[2048];
    sprintf(response,
            "HTTP/1.0 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 26\r\n"
            "\r\n"
            "<h1>%d</h1>",
            counter);

    write(client_socket_fd, response, sizeof(response));

    close(client_socket_fd);
  }

  close(socketfd);

  return 0;
}
