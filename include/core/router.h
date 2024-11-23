#include <stdbool.h>
#include "../http/http_types.h"
#include "../utils/array_list.h"

typedef struct Node {
  struct Hashmap *children;
  char *(*call_methods[4])(List*);
  char *key;
  bool dynamic;
} Node;

typedef struct Hashmap {
  Node **arr;
  unsigned int fullfiled_slots;
  unsigned int size;
} Hashmap;

Hashmap *init_hashmap();
void insert(struct Hashmap *, enum HTTP_METHOD, char *, char *(*handle_response)());
struct Node *get_from_hashmap(struct Hashmap *, char *);

typedef struct Router {
  Node *root_node;
} Router;

Router *init_router();
void free_router(Router *);

void add_endpoint(struct Router *, enum HTTP_METHOD, char *, char *(*handle_response)());
char *call_endpoint(struct Router*, HTTP_REQUEST*);
