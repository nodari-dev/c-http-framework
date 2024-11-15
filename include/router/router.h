#include <stdbool.h>

#include "router_hashmap.h"
#include "../http_types.h"

typedef struct Router {
	Hashmap* roots;
	void (*add_endpoint)(enum HTTP_METHOD, char*, struct Router*);
} Router;

typedef struct Router_Node {
	Hashmap children;
	char *value;
	bool dynamic;
} Router_Node;

Router *init_router();
void free_router();
