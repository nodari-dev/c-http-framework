enum HTTP_METHOD{
	POST, GET, PUT, DELETE, UNSUPPORTED
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

