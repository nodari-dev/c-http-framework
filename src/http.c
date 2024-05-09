#include <stdio.h>
#include <string.h>
#include "../include/conf.h"

void read_http_request(char *buffer) {
  char method[BUFFER_FOR_READ];
  char uri[BUFFER_FOR_READ];
  char version[BUFFER_FOR_READ];
  sscanf(buffer, "%s %s %s", method, uri, version);

  printf("%s\n", buffer);

  printf("Request:\n");
  printf("%s %s %s\n", method, uri, version);

  char *request_start, *header_start, *body_start;

  request_start = strstr(buffer, "\r\n\r\n");

  // TODO: check if not NULL
  header_start = strstr(buffer, "\r\n\r\n");
  header_start += 4;

  printf("Headers:\n");
  printf("%s\n", header_start);

  // TODO: check if not NULL
  body_start = strstr(header_start, "\r\n");

  printf("Body:\n");
  if (body_start == NULL) {
    printf("NO FUCKING BODY\n");
  } else {
    printf("%s\n", body_start);
  }
}
