#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/router/router.h"

#define MIN_FULLFILMENT_PERSENTAGE 20
#define MAX_FULLFILMENT_PERSENTAGE 80

Node *create_node(char *key) {
  Node *h_item = malloc(sizeof(Node));
  if (h_item == NULL) {
    exit(1);
  }
  h_item->key = key;
  h_item->children = init_hashmap();

  return h_item;
}

unsigned int sdbm_hash_me_dady(char *str, unsigned int length,
                               unsigned int hmap_size) {
  unsigned int hash = 0;
  unsigned int i = 0;

  for (i = 0; i < length; str++, i++) {
    hash = (*str) + (hash << 6) + (hash << 16) - hash;
  }

  return hash % hmap_size;
};

void resize(Hashmap *hmap, unsigned int new_size) {
  Node **new_arr = malloc(sizeof(Node) * new_size);
  if (new_arr == NULL) {
    exit(1);
  }

  for (int i = 0; i < hmap->size; ++i) {
    if (hmap->arr[i] != NULL) {
      unsigned int new_hash = sdbm_hash_me_dady(
          hmap->arr[i]->key,
          strnlen(hmap->arr[i]->key, sizeof hmap->arr[i]->key), new_size);

      new_arr[new_hash] = hmap->arr[i];
    }
  }
}

void resize_or_skip(Hashmap *hmap) {
  if ((hmap->fullfiled_slots / hmap->size) * 100 ==
      MIN_FULLFILMENT_PERSENTAGE) {
    resize(hmap, hmap->size * 2);
  }

  if ((hmap->fullfiled_slots / hmap->size) * 100 ==
      MAX_FULLFILMENT_PERSENTAGE) {
    resize(hmap, hmap->size / 2);
  }
}

void add_endpoint(struct Router *r, enum HTTP_METHOD method, char *path,
                  char *(*handle_response)()) {

  // if (strchr(path, ':') == NULL) {
  //   insert_static_endpoint_into_hashmap(r->static_endpoints, method, path,
  //                                       handle_response);
  // } else {
  //   insert_dynamic_endpoint_into_hashmap(r->dynamic_endpoints, method, path,
  //                                        handle_response);
  // }
  //

  //  } else if (hmap->arr[hash]->key == key &&
  // 		 hmap->arr[hash]->call_methods[method] == NULL) {
  // // same endpoint, different method
  // hmap->arr[hash]->call_methods[method] = handle_response;
  //  } else if (hmap->arr[hash]->key == key &&
  // 		 hmap->arr[hash]->call_methods[method] != NULL) {
  // printf("Overriding endpoint, check your code, you dummy\n");
  // exit(1);
  //  } else {
  // // in case if two endpoints collide, I need to resize the hashmap
  // resize(hmap, hmap->size * 2);
  //  }
  char *buffer = strdup(path);
  char *token;
  token = strtok(buffer, "/");
  Node *current_node = r->root_node;

  while (token != NULL) {
    Node *new_node = create_node(token);
    unsigned int hash =
        sdbm_hash_me_dady(token, strnlen(token, sizeof token), current_node->children->size);

    if (current_node->children->arr[hash] == NULL) {
      current_node->children->arr[hash] = new_node;
      current_node->children->fullfiled_slots++;
	  resize_or_skip(current_node->children);
    }
    current_node = current_node->children->arr[hash];
    token = strtok(NULL, "/");
  }

  if (current_node->call_methods[method] != NULL) {
    printf(
        "You are overriding existing endpoint, check your code, you dummy\n");
    exit(1);
  }
  current_node->call_methods[method] = handle_response;
}

char *call_endpoint(struct Router *r, enum HTTP_METHOD method, char *path) {
  //
  // Node *endpoint = get_from_hashmap(hm, path);
  // if (endpoint == NULL) {
  //   return "nothing found";
  // }
  // if (endpoint->call_methods[method] == NULL) {
  //   return "nothing found for method";
  // }
  // return endpoint->call_methods[method]();
  return "";
}

Router *init_router() {
  Router *r = malloc(sizeof(Router));
  if (r == NULL) {
    perror("Router malloc");
    exit(1);
  }

  r->root_node = create_node("");

  return r;
}

void free_router(Router *r) {
}

void insert(Hashmap *hmap, enum HTTP_METHOD method, char *key,
            char *(*handle_response)()) {
  Node *new_h_item = create_node(key);
  new_h_item->call_methods[method] = handle_response;

  unsigned int hash =
      sdbm_hash_me_dady(key, strnlen(key, sizeof key), hmap->size);

  // if (hmap->arr[hash] == NULL) {
  //   hmap->arr[hash] = new_h_item;
  //   hmap->fullfiled_slots++;
  //   return;
  // } else if (hmap->arr[hash]->key == key &&
  //            hmap->arr[hash]->call_methods[method] == NULL) {
  //   // same endpoint, different method
  //   hmap->arr[hash]->call_methods[method] = handle_response;
  // } else if (hmap->arr[hash]->key == key &&
  //            hmap->arr[hash]->call_methods[method] != NULL) {
  //   printf("Overriding endpoint, check your code, you dummy\n");
  //   exit(1);
  // } else {
  //   // in case if two endpoints collide, I need to resize the hashmap
  //   resize(hmap, hmap->size * 2);
  // }

  resize_or_skip(hmap);
}

Node *get_from_hashmap(Hashmap *hmap, char *key) {
  unsigned int hash =
      sdbm_hash_me_dady(key, strnlen(key, sizeof key), hmap->size);
  return hmap->arr[hash];
}

Hashmap *init_hashmap() {
  Hashmap *hashmap = malloc(sizeof(Hashmap));
  if (hashmap == NULL) {
    exit(1);
  }

  hashmap->fullfiled_slots = 0;
  hashmap->size = 16;

  hashmap->arr = malloc(sizeof(Node) * hashmap->size);
  if (hashmap->arr == NULL) {
    free(hashmap);
    exit(1);
  }

  for (int i = 0; i < hashmap->size; ++i) {
    hashmap->arr[i] = NULL;
  }

  return hashmap;
}
