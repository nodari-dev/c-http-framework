#include <endian.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#include "../../include/conf.h"
#include "../../include/http/http.h"

void free_http_request(struct HTTP_REQUEST *http_request);

HTTP_REQUEST *parse_http_request(char *buffer) {
  if (!buffer) {
	  printf("Buffer not found\n");
    return NULL;
  }

  HTTP_REQUEST *http_request =
      (struct HTTP_REQUEST *)malloc(sizeof(HTTP_REQUEST));
  if (!http_request) {
    perror("malloc http request");
    return NULL;
  }

  // METHOD
  size_t method_len = strcspn(buffer, " ");
  if (memcmp(buffer, "POST", strlen("POST")) == 0) {
    http_request->method = POST;
  }
  else if (memcmp(buffer, "GET", strlen("GET")) == 0) {
    http_request->method = GET;
  }
  else if (memcmp(buffer, "PUT", strlen("PUT")) == 0) {
    http_request->method = PUT;
  }
  else if (memcmp(buffer, "DELETE", strlen("DELETE")) == 0) {
    http_request->method = DELETE;
  } else {
    http_request->method = UNSUPPORTED;
  }

  // move to the next item
  buffer += method_len + 1;

  // URI
  size_t uri_len = strcspn(buffer, " ");
  http_request->uri = malloc(uri_len + 1);
  if (!http_request->uri) {
    perror("malloc uri");
    free(http_request);
    return NULL;
  }

  memcpy(http_request->uri, buffer, uri_len);
  http_request->uri[uri_len] = '\0';

  // move to the next item
  buffer += uri_len + 1;

  // VERSION
  size_t version_len = strcspn(buffer, "\r\n");
  http_request->version = malloc(version_len + 1);
  if (!http_request->version) {
    perror("malloc version");
    free_http_request(http_request);
    return NULL;
  }

  memcpy(http_request->version, buffer, version_len);
  http_request->version[version_len] = '\0';
  buffer += version_len + 2;

  // HEADERS
  struct HTTP_HEADER *current_http_header = NULL, *last_http_header = NULL;

  while (buffer[0] != '\r' || buffer[1] != '\n') {
    last_http_header = current_http_header;
    current_http_header = (HTTP_HEADER *)malloc(sizeof(HTTP_HEADER));
    if (!current_http_header) {
      perror("malloc http header");
      free_http_request(http_request);
      return NULL;
    }

    size_t name_len = strcspn(buffer, ":");
    current_http_header->name = malloc(name_len + 1);
    if (!current_http_header->name) {
      perror("malloc header name");
      free_http_request(http_request);
      return NULL;
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
      perror("malloc header value");
      free_http_request(http_request);
      return NULL;
    }

    memcpy(current_http_header->value, buffer, value_len);
    current_http_header->value[value_len] = '\0';

    buffer += value_len + 2;
    current_http_header->next = last_http_header;
  }

  http_request->headers = current_http_header;

  // skip line which separates headers and body
  buffer += 2;

  // BODY
  size_t body_len = strlen(buffer);
  if (body_len != 0) {
    http_request->body = (char *)malloc(body_len * sizeof(char));
    if (!http_request->body) {
      printf("could not allocate memory for http body");
      free_http_request(http_request);
      return NULL;
    }

    memcpy(http_request->body, buffer, body_len);
    http_request->body[body_len] = '\0';
  }

  return http_request;
}

char* http_method_to_str(enum HTTP_METHOD method){
	switch (method) {
	  case GET: return "GET";
	  case PUT: return "PUT";
	  case POST: return "POST";
	  case DELETE: return "DELETE";
	  case UNSUPPORTED: return "UNSUPPORTED";
	}
}

void free_header(HTTP_HEADER *header) {
  if (header) {
    free(header->name);
    header->name = NULL;

    free(header->value);
    header->value = NULL;

    struct HTTP_HEADER *next_header = header->next;
    header->next = NULL;

    free(header);
    header = NULL;

    free_header(next_header);
  }
}
void free_http_request(HTTP_REQUEST *http_request) {
  if (http_request) {
    free(http_request->uri);
    http_request->uri = NULL;
    free(http_request->version);
    http_request->version = NULL;
    free(http_request->body);
    http_request->body = NULL;
    // free_header(http_request->headers);
    free(http_request);
    http_request = NULL;
  }
}
