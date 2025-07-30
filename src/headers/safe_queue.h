#ifndef safe_queue
#define safe_queue

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE 100

typedef struct {
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_occupancy;
} Queue;

Queue* create_queue();
bool is_empty(Queue* queue);
void enqueue(Queue* queue, int item);
int dequeue(Queue* queue);
void destroy_queue(Queue* queue);

#endif