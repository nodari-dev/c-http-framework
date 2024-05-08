struct Queue{
	int len;
	struct Node *head;
	struct Node *tail;
};
struct Queue* createQueue();
void enque(struct Queue *queue, int value);
int deque(struct Queue *queue);
void monitor_queue(struct Queue *queue);
