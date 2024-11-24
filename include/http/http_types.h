
#ifndef _HTTPTYPESH 
#define _HTTPTYPESH 

enum HTTP_METHOD{
	POST, GET, PUT, DELETE, UNSUPPORTED
};

enum HTTP_STATUS_CODES{
	OK = 200,
	BAD_REQUEST = 400,
	NOT_FOUND = 404,
	INTERNAL_SERVER_ERROR = 500,
};

typedef struct HTTP_HEADER{
	char *name;
	char *value;
	struct HTTP_HEADER *next;
} HTTP_HEADER;

typedef struct HTTP_REQUEST{
	char *uri;
	char *version;
	enum HTTP_METHOD method;
	struct HTTP_HEADER *headers;
	char *body;

} HTTP_REQUEST;

char* parse_http_method(enum HTTP_METHOD);

#endif

