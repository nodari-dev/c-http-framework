#include <stdbool.h>
#include "../http_types.h"

typedef struct Endpoint {
  struct Hashmap *children;
  char *(*call_methods[4])();
  char *key;
} Endpoint;

typedef struct Hashmap {
  Endpoint **arr;
  unsigned int fullfiled_slots;
  unsigned int size;
} Hashmap;

Hashmap *init_hashmap();
void insert_into_hashmap(struct Hashmap *, enum HTTP_METHOD, char *, char *(*handle_response)());
struct Endpoint *get_from_hashmap(struct Hashmap *, char *);

typedef struct Router {
  Hashmap *static_endpoints;
  Hashmap *dynamic_endpoints;
} Router;

Router *init_router();
void free_router(Router *);

void add_endpoint(struct Router *, enum HTTP_METHOD, char *, char *(*handle_response)());
char *call_endpoint(struct Router*, enum HTTP_METHOD, char *);
