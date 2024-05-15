
// main file
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
      while (header) {
        printf("%s: %s\n", header->name, header->value);
        header = header->next;
      }
      if (http_request->body) {
        printf("%s\n", http_request->body);
      }
    } else {
      printf("%s\n", "parsing error");
    }

    enque(request_queue, counter);

	//  AFTER
	free(buffer);
	buffer = NULL;
	free_http_request(http_request);
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

// http parser

void free_http_request(struct HTTP_REQUEST *http_request);

HTTP_REQUEST *read_http_request(char *buffer) {
  HTTP_REQUEST *http_request = (struct HTTP_REQUEST*)malloc(sizeof(HTTP_REQUEST));
  if (!http_request) {
    perror("malloc http request");
    return NULL;
  }

  // METHOD
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

void free_header(struct HTTP_HEADER *header) {
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
void free_http_request(struct HTTP_REQUEST *http_request) {
  if (http_request) {
    free(http_request->uri);
	http_request->uri = NULL;
    free(http_request->version);
	http_request->version = NULL;
    free(http_request->body);
	http_request->body = NULL;
    free_header(http_request->headers);
	free(http_request);
	http_request = NULL;
  }
}

// logs after sending 4 requests from postman, in the end, body from 2nd and 3rd request merged into one, which is incorrect

Server is running on host: 127.0.0.1
4 /123 HTTP/1.1
Postman-Token: dfe06161-d6f7-4f43-ae85-d1eb5ae7fa86
{
    "a": 1
}
4 /123 HTTP/1.1
Postman-Token: 825ace66-675a-4557-a230-e9a8b93ea78b
{
    "a": 1,
    "b": 2
}
4 /123 HTTP/1.1
Postman-Token: 7aef30ef-51cf-4378-872b-a1b2bc201360
{
    "a": 1
}    "b": 2
}
4 /123 HTTP/1.1
Postman-Token: 4ec75c01-3154-4990-bbd8-3fcc694af474
{
    "a": 1
}   "b": 2
}

