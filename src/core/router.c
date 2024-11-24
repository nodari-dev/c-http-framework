#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/core/router.h"
#include "../../include/http/http.h"

#define MIN_FULLFILMENT_PERSENTAGE 20
#define MAX_FULLFILMENT_PERSENTAGE 80

Node *create_node(char *key) {
  Node *h_item = malloc(sizeof(Node));
  if (h_item == NULL) {
    exit(1);
  }

  h_item->key = key;
  if (strchr(key, ':') != NULL) {
    h_item->dynamic = true;
  }

  h_item->children = init_hashmap();

  return h_item;
}

unsigned int sdbm_hash_me_dady(char *str, unsigned int hmap_size) {
  unsigned int hash = 0;
  unsigned int i = 0;
  while (*str) {
    hash = (*str++) + (hash << 6) + (hash << 16) - hash;
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
      unsigned int new_hash = sdbm_hash_me_dady(hmap->arr[i]->key, new_size);

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

  char *buffer = strdup(path);
  char *token;
  token = strtok(buffer, "/");
  Node *current_node = r->root_node;

  while (token != NULL) {
    Node *new_node = create_node(token);
    unsigned int hash = sdbm_hash_me_dady(token, current_node->children->size);
printf("%d\n", hash);

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

char *call_endpoint(struct Router *r, struct HTTP_REQUEST *req) {
  char *buffer = strdup(req->uri);
  char *token;
  token = strtok(buffer, "/");
  Node *current_node = r->root_node;
  List *list = init_list();

  while (token != NULL) {
    unsigned int hash = sdbm_hash_me_dady(token, current_node->children->size);

    current_node = current_node->children->arr[hash];
    if (current_node == NULL) {
      return gen_http_resp(404, NULL);
    }

    if (current_node->dynamic == true) {
      push(list, token);
    }

    token = strtok(NULL, "/");
  }

  if (list->last_index == -1) {
    free_list(list);
  }

  if (current_node->call_methods[req->method] == NULL) {
    return gen_http_resp(404, NULL);
  }

  return current_node->call_methods[req->method](list);
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
  // PUT CODE HERE
}

Hashmap *init_hashmap() {
  Hashmap *hashmap = (Hashmap *)malloc(sizeof(Hashmap));
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
