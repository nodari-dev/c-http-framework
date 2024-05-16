#include "http_types.h"

HTTP_REQUEST* parse_http_request(char *buffer);
void free_http_request(struct HTTP_REQUEST *http_request);
