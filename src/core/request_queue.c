#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/core/request_queue.h"

Q *createQueue() {
  Q *queue = (Q *)malloc(sizeof(Q));
  if (queue == NULL) {
    perror("request_queue malloc");
    exit(1);
  }
  queue->head = NULL;
  queue->tail = NULL;
  queue->len = 0;
  return queue;
}

QN *createNode(int client_socket_fd) {
  QN *newNode = (QN *)malloc(sizeof(QN));
  if (newNode == NULL) {
    perror("queue new node");
    exit(1);
  }
  newNode->client_socket_fd = client_socket_fd;
  newNode->next = NULL;
  return newNode;
}

void enque(Q *queue, int client_socket_fd) {
  QN *newNode = createNode(client_socket_fd);

  if (queue->len == 0) {
    queue->head = queue->tail = newNode;
    queue->len++;
	return;
  }

  queue->len++;
  queue->tail->next = newNode;
  queue->tail = newNode;
}

int deque(Q *queue) {
  if (queue->head == NULL) {
	  return -1;
  }

  QN *temp = queue->head;
  int client_socket_fd = temp->client_socket_fd;
  queue->head = queue->head->next;
  if (queue->head == NULL) {
    queue->tail = NULL;
  }
  free(temp);
  queue->len--;

  return client_socket_fd;
}

void free_request_queue(Q* q){
	if(q->head == NULL){
		free(q);
		return;
	}

	QN* curr = q->head;
	while(curr->next != NULL){
		curr = curr->next;
		free(curr);
	}
	free(curr);

}

void monitor_queue(Q *queue) {
  QN *current = queue->head;
  int counter = 0;

  while (current != NULL) {
    printf("Q[%d] %d\n", counter, current->client_socket_fd);
    current = current->next;
    counter++;
  }
}
