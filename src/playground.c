#include "../include/conf.h"
#include "../include/errors.h"
#include "../include/http_parser.h"
#include "../include/request_queue.h"
#include "../include/request_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct HTTP_REQUEST* test() {
  struct HTTP_REQUEST *request;
  request = (struct HTTP_REQUEST *)malloc(sizeof(struct HTTP_REQUEST));
  return request;
}

int main() {
  for (int i = 0; i < 3; ++i) {
    struct HTTP_REQUEST *http_request = test();

    if (http_request != NULL) {
      printf("Allocation %d, %p\n", i, http_request);
    }

  }

  return 0;
}
