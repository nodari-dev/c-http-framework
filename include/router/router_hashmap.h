typedef struct Hashmap_item {
  char *key;
  char *value;
} Hashmap_item;

typedef struct Hashmap {
  Hashmap_item **arr;
  unsigned int fullfiled_slots;
  unsigned int size;
  void (*insert)(struct Hashmap *hmap, char *key, char *value);
  struct Hashmap_item* (*get)(struct Hashmap *hmap, char *key);
} Hashmap;

Hashmap *init_hashmap();
