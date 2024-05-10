#include <endian.h>
#include <stdio.h>
#include <string.h>

#include "../include/conf.h"

void read_http_request(char *buffer) {
  char method[512];
  char uri[512];
  char version[512];

  size_t method_len = strcspn(buffer, " ");
  if (memcmp(buffer, "GET", strlen("GET")) == 0) {
    strcpy(method, "GET");
  } else {
    strcpy(method, "ANOTHER");
  }

  method[method_len] = '\0';
  // move to the next item
  buffer += method_len + 1;

  size_t uri_len = strcspn(buffer, " ");
  strncpy(uri, buffer, uri_len);
  // move to the next item
  buffer += uri_len + 1;
  uri[uri_len] = '\0';

  size_t version_len = strcspn(buffer, "\r\n");
  strncpy(version, buffer, version_len);
  buffer += version_len + 2;
  version[version_len] = '\0';
  printf("%s %s %s\n", method, uri, version);

  while (buffer[0] != '\0' || buffer[0] != '\r' || buffer[1] != '\n') {
    char name[512];
    char value[512];
    size_t name_len = strcspn(buffer, ":");
    strncpy(name, buffer, name_len);

    name[name_len] = '\0';
    // move from :
    buffer += name_len + 1;

    // move until finds value of a header
    while (*buffer == ' ') {
      ++buffer;
    }

    size_t value_len = strcspn(buffer, "\r\n");
	strncpy(value, buffer, value_len);

	value[value_len] = '\0';
	buffer += value_len + 2;

	printf("%s:%s\n", name, value);
  }

  // get to new line to body
  buffer += 2;

  char body[512];
  size_t body_len = strlen(buffer);
  strncpy(body, buffer, body_len);

  body[body_len] = '\0';

  printf("%s\n", body);
  
}
