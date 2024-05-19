#include <unistd.h>  
#include <pthread.h> 

#include "../include/request_queue.h"
#include "../include/thread_pool.h"

int main() {
	Request_Queue* request_queue = createQueue();
	enque(request_queue, 1);
	monitor_queue(request_queue);
	Thread_Pool* thread_pool = init_thread_pool(request_queue);

  return 0;
}
