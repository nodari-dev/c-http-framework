#include <stdio.h>
#include <stdlib.h>

struct Node{
	struct Node *next;
	int val;
};

struct Queue{
	int len;
	struct Node *head;
	struct Node *tail;
};

struct Node* createNode(int value){
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->val = value;
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

void enque(struct Queue *queue, int value){
	struct Node* newNode = createNode(value);
	if(queue->tail == NULL){
		queue->head = queue->tail = newNode;
		return;
	}

	queue->tail->next = newNode;
	queue->tail = newNode;
}

int deque(struct Queue *queue){
	int result;
	if(queue->head == NULL){
		result = -1;
	} else{
		struct Node* temp = queue->head;
		result = temp->val; 
		queue->head = queue->head->next;
		if(queue->head == NULL){
			queue->tail = NULL;
		}
		free(temp);
	}

	return result;
}

void monitor_queue(struct Queue *queue){
	struct Node *current = queue->head;

	printf("Request queue\n");

	while (current != NULL) {
		printf("%d, ", current->val);
		current = current->next;
	}

	printf("\n");

}
