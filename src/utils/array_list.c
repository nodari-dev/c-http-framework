#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/utils/array_list.h"

List *init_list() {
  List *list = malloc(sizeof(List));
  if (list == NULL) {
    perror("init_list malloc\n");
    exit(1);
  }

  list->size = 8;
  list->last_index = -1;

  list->arr = malloc(sizeof(char) * list->size);
  if (list->arr == NULL) {
    free_list(list);
    perror("init_list array malloc\n");
    exit(1);
  }

  return list;
}

void resize_list(List *list) {
  char **new_arr = malloc(sizeof(char *) * list->size * 2);
  for (int i = 0; i < list->size; i++) {
    new_arr[i] = strdup(list->arr[i]);
    if (new_arr[i] == NULL) {
      perror("init_list array malloc\n");
      exit(1);
    }
  }

  list->size *= 2;

  free(list->arr);
  list->arr = new_arr;
}

void push(List *list, char *item) {
  if (list->size == list->last_index + 1) {
    resize_list(list);
  }

  list->last_index++;
  list->arr[list->last_index] = strdup(item);

  if (list->arr[list->last_index] == NULL) {
    perror("push null\n");
    exit(1);
  }
}

void free_list(List *list) {
  for (int i = 0; i <= list->last_index; i++) {
    free(list->arr[i]);
  }
  free(list->arr);
  free(list);
}
