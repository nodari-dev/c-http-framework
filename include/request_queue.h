struct Queue{
	int len;
	struct Node *head;
	struct Node *tail;
};
struct Queue* createQueue();
void enque(struct Queue *queue, struct HTTP_REQUEST* request);
struct HTTP_REQUEST* deque(struct Queue *queue);
void monitor_queue(struct Queue *queue);
