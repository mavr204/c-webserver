#include "safe_queue.h"

Queue* create_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        perror("Failed to create queue");
        exit(EXIT_FAILURE);
    }
    queue->front = 0;
    queue->rear = 0;
    return queue;
}

bool is_empty(Queue* queue) {

    return queue->front == queue->rear;
}

void enqueue(Queue* queue, Task item) {
    pthread_mutex_lock(&queue->queue_lock);
    
    if ((queue->rear +1) % MAX_QUEUE_SIZE == queue->front) {
        printf("Queue is full, cannot enqueue %d\n", item);
        pthread_mutex_unlock(&queue->queue_lock);
        return;
    }
    queue->tasks[queue->rear] = item;
    queue->rear = (queue->rear +1) % MAX_QUEUE_SIZE;
    pthread_cond_signal(&queue->queue_occupancy);
    pthread_mutex_unlock(&queue->queue_lock);
    printf("Enqueued item: %d\n", item);
}

Task dequeue(Queue* queue){
    pthread_mutex_lock(&queue->queue_lock);

    while (is_empty(queue)) {
        pthread_cond_wait(&queue->queue_occupancy, &queue->queue_lock);
    }

    Task item = queue->tasks[queue->front];
    printf("Dequeued item: %d\n", item);  
    queue->front = (queue->front +1) % MAX_QUEUE_SIZE;
    pthread_mutex_unlock(&queue->queue_lock);
    return item;
}

void destroy_queue(Queue* queue) {
    free(queue);
    printf("Queue destroyed.\n");
}