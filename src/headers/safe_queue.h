#ifndef safe_queue
#define safe_queue

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE 100

typedef struct Task{
    void* (*task_function)(void*);
    void* arg;
    int client_fd;
} Task;

typedef struct {
    Task tasks[MAX_QUEUE_SIZE];
    int front;
    int rear;
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_occupancy;
} Queue;

Queue* create_queue();
bool is_empty(Queue* queue);
void enqueue(Queue* queue, Task item);
Task dequeue(Queue* queue);
void destroy_queue(Queue* queue);

#endif