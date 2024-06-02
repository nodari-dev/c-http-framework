#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/conf.h"
#include "../include/errors.h"
#include "../include/request_queue.h"

#include "../include/conf.h"

char *read_request(int client_socket_fd) {
  int predefined_buffer_size = BUFFER_FOR_READ;
  char *buffer = malloc(predefined_buffer_size);
  int total_buffer_size = 0;
  int bytes_read;

  if (!buffer) {
    perror("buffer malloc");
    exit(1);
  }

  // NOTE: buffer + total_buffer_size - go to free space slot
  // NOTE: predefined_buffer_size - total_buffer_size - size left
  while (1) {
    bytes_read = recv(client_socket_fd, buffer + total_buffer_size,
                      predefined_buffer_size - total_buffer_size, 0);

    if (bytes_read <= 0) {
		break;
    }

    total_buffer_size += bytes_read;
    if (total_buffer_size >= predefined_buffer_size) {
      predefined_buffer_size *= 2;
      char *new_buffer = realloc(buffer, predefined_buffer_size);

      if (new_buffer == NULL) {
        perror("new_buffer error realloc");
        free(buffer);
        buffer = NULL;
        exit(1);
      }

      buffer = new_buffer;
    }

    buffer = realloc(buffer, total_buffer_size + bytes_read);
    if (strstr(buffer, "\r\n\r\n")) {
      break;
    }
  }

  char *temp = realloc(buffer, total_buffer_size + 1);
  if (temp == NULL) {
    perror("realloc");
    free(buffer);
    buffer = NULL;
    exit(1);
  }

  buffer = temp;
  buffer[total_buffer_size] = '\0';
  return buffer;
}
