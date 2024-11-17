#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/router/router.h"

#define MIN_FULLFILMENT_PERSENTAGE 20
#define MAX_FULLFILMENT_PERSENTAGE 80

void add_endpoint(struct Router *r, enum HTTP_METHOD method, char *path, char *(*handle_response)()) {

  // /profile/:id
  // /profile/all

  // if : is there -> dynamic
  insert_into_hashmap(r->static_endpoints, method, path, handle_response);
  // r->dynamic_endpoints->insert(r->dynamic_endpoints, "path");
}

char *call_endpoint(struct Router *r, enum HTTP_METHOD method, char *path) {
  Hashmap *hash = r->static_endpoints;
  
  Endpoint *endpoint = get_from_hashmap(hash, path);
  if(endpoint == NULL){
	  return "nothing found";
  }
  if(endpoint->call_methods[method] == NULL){
	  return "nothing found for method";
  }
  return endpoint->call_methods[method]();
}

Router *init_router() {
  Router *r = malloc(sizeof(Router));
  if (r == NULL) {
    perror("Router malloc");
    exit(1);
  }

  r->static_endpoints = init_hashmap();
  r->dynamic_endpoints = init_hashmap();

  return r;
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

void free_router(Router *r){
	for(int i = 0; i < r->static_endpoints->size; ++i){
		if(r->static_endpoints->arr[i] != NULL){
			free(r->static_endpoints->arr[i]);
		}
	}
	free(r->static_endpoints);

	// TODO: ADD FREE FOR DYNAMIC ENDPOINTS
	
	free(r->dynamic_endpoints);
}

Endpoint *create_endpoint(char *key) {
  Endpoint *h_item = malloc(sizeof(Endpoint));
  if (h_item == NULL) {
    exit(1);
  }
  h_item->key = key;

  return h_item;
}

void resize(Hashmap *hmap, unsigned int new_size) {
  Endpoint **new_arr = malloc(sizeof(Endpoint) * new_size);
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

void insert_into_hashmap(Hashmap *hmap, enum HTTP_METHOD method, char *key, char *(*handle_response)()) {
  Endpoint *new_h_item = create_endpoint(key);
  new_h_item->call_methods[method] = handle_response;

  unsigned int hash =
      sdbm_hash_me_dady(key, strnlen(key, sizeof key), hmap->size);

  if (hmap->arr[hash] == NULL) {
    hmap->arr[hash] = new_h_item;
    hmap->fullfiled_slots++;
	return;
  } else if (hmap->arr[hash]->key == key && hmap->arr[hash]->call_methods[method] == NULL) {
	  // same endpoint, different method
	  hmap->arr[hash]->call_methods[method] = handle_response;
  } else if(hmap->arr[hash]->key == key && hmap->arr[hash]->call_methods[method] != NULL){
	  printf("Overriding endpoint, check your code, you dummy\n");
	  exit(1);
  } else {
    // in case if two endpoints collide, I need to resize the hashmap
    resize(hmap, hmap->size * 2);
  }

  resize_or_skip(hmap);
}

Endpoint *get_from_hashmap(Hashmap *hmap, char *key) {
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
  hashmap->size = 32;

  hashmap->arr = malloc(sizeof(Endpoint) * hashmap->size);
  if (hashmap->arr == NULL) {
    free(hashmap);
    exit(1);
  }

  for (int i = 0; i < hashmap->size; ++i) {
    hashmap->arr[i] = NULL;
  }

  return hashmap;
}

