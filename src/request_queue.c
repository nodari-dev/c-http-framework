#include <stdio.h>
#include <stdlib.h>
#include "../include/http_types.h"

struct Node{
	struct Node *next;
	struct HTTP_REQUEST* http_request;
};

struct Queue{
	int len;
	struct Node *head;
	struct Node *tail;
};

struct Node* createNode(struct HTTP_REQUEST* request){
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	if(newNode == NULL){
		perror("queue new node");
		exit(1);
	}
	newNode->http_request = request;
	newNode->next = NULL;
	return newNode;
}

struct Queue* createQueue(){
	struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
	queue->head = NULL;
	queue->tail = NULL;
	queue->len = 0;
	return queue;
}

void enque(struct Queue *queue, struct HTTP_REQUEST* request){
	struct Node* newNode = createNode(request);
	if(queue->len == 0){
		queue->head = queue->tail = newNode;
		return;
	}
	queue->len++;
	queue->tail->next = newNode;
	queue->tail = newNode;
}

struct HTTP_REQUEST* deque(struct Queue *queue){
	struct HTTP_REQUEST* node = NULL;
	if(queue->len > 0){
		struct Node* temp = queue->head;
		node = temp->http_request; 
		queue->head = queue->head->next;
		if(queue->head == NULL){
			queue->tail = NULL;
		}
		free(temp);
		queue->len--;
	} 

	return node;
}

void monitor_queue(struct Queue* queue){
	struct Node *current = queue->head;
	int counter = 0;

	while (current != NULL) {
		printf("Q[%d] %s\n", counter, current->http_request->uri);
		current = current->next;
		counter++;
	}
}
