#include "http_types.h"

#ifndef _HTTPPARSERH 
#define _HTTPPARSERH 

HTTP_REQUEST* parse_http_request(char *buffer);
char* http_method_to_str(enum HTTP_METHOD);
void free_http_request(struct HTTP_REQUEST *http_request);

#endif
