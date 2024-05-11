#include <endian.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/conf.h"
#include "../include/types.h"

void read_http_request(char *buffer) {
  HTTP_REQUEST *http_request = (HTTP_REQUEST *)calloc(0, sizeof(HTTP_REQUEST));
  if (!http_request) {
    printf("Could not allocate memory for http request");
    return;
  }

  size_t method_len = strcspn(buffer, " ");
  if (memcmp(buffer, "POST", strlen("POST")) == 0) {
    http_request->method = POST;
  }
  if (memcmp(buffer, "GET", strlen("GET")) == 0) {
    http_request->method = GET;
  }
  if (memcmp(buffer, "PUT", strlen("PUT")) == 0) {
    http_request->method = PUT;
  }
  if (memcmp(buffer, "DELETE", strlen("DELETE")) == 0) {
    http_request->method = DELETE;
  } else {
    http_request->method = UNSUPPORTED;
  }

  // move to the next item
  buffer += method_len + 1;

  size_t uri_len = strcspn(buffer, " ");
  http_request->uri = malloc(uri_len + 1);
  if (!http_request->uri) {
    printf("Could not allocate memory for uri");
    return;
  }

  memcpy(http_request->uri, buffer, uri_len);
  http_request->uri[uri_len] = '\0';

  // move to the next item
  buffer += uri_len + 1;

  size_t version_len = strcspn(buffer, "\r\n");
  http_request->version = malloc(version_len + 1);
  if (!http_request->version) {
    printf("Could not allocate memory for version");
    return;
  }

  memcpy(http_request->version, buffer, version_len);
  http_request->version[version_len] = '\0';
  buffer += version_len + 2;
  printf("%d %s %s\n", http_request->method, http_request->uri,
         http_request->version);

  struct HTTP_HEADER *current_http_header = NULL, *last_http_header = NULL;

  while (buffer[0] != '\r' || buffer[1] != '\n') {
	last_http_header = current_http_header;
    current_http_header = (HTTP_HEADER *)calloc(0, sizeof(HTTP_REQUEST));
    if (!current_http_header) {
      printf("could not allocate memory for http header");
      return;
    }


    size_t name_len = strcspn(buffer, ":");
    current_http_header->name = malloc(name_len + 1);
    if (!current_http_header->name) {
      printf("could not allocate memory for http header name");
      return;
    }

    memcpy(current_http_header->name, buffer, name_len);
    current_http_header->name[name_len] = '\0';

    // move from :
    buffer += name_len + 1;

    // move until finds value of a header
    while (*buffer == ' ') {
      ++buffer;
    }

    size_t value_len = strcspn(buffer, "\r\n");

    current_http_header->value = malloc(value_len + 1);
    if (!current_http_header->value) {
      printf("could not allocate memory for http header value");
      return;
    }
    memcpy(current_http_header->value, buffer, value_len);
    current_http_header->value[value_len] = '\0';

    buffer += value_len + 2;

	current_http_header->next = last_http_header;
  }
  http_request->headers = current_http_header;

  // get to new line to body
  buffer += 2;

  size_t body_len = strlen(buffer);
  http_request->body = malloc(body_len + 1);
  memcpy(http_request->body, buffer, body_len);

  http_request->body[body_len] = '\0';

  struct HTTP_HEADER *header = http_request->headers;
  while (header->next) {
    printf("%s: %s\n", header->name, header->value);
    header = header->next;
  }

  printf("%s\n", http_request->body);
}
