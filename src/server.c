#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "safe_queue.h"
#include "request_handler.h"

#define PORT 8080
#define BACKLOG 5

int get_socket();
void start_server(int sockfd);
int get_client(int sockfd);
// void* client_handler(void* arg);
void close_client(int client_fd);
void close_server(int sockfd);
char* read_file(const char* filename, size_t* length);
void thread_pool_init(Queue *queue);

int sockfd;

int main()
{
    Queue *queue = create_queue();
    thread_pool_init(queue);
    int client_fd = -1;
    sockfd = get_socket();
    start_server(sockfd);

    for (;;)
    {
        client_fd = get_client(sockfd);
        if (client_fd < 0)
        {
            continue;
        }
        //=========================================================review=====================================================
        // int* fd_ptr = malloc(sizeof(int));
        // *fd_ptr = client_fd;
        // Task* task = (Task *)malloc(sizeof(Task));
        // task->client_fd = client_fd;
        // task->task_function = serve_home;
        // task->arg = (void*)fd_ptr;
        // enqueue(queue, task);
        // free(task);
    }
}

int get_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket returns a file descriptor. AF_INET is for IPv4, SOCK_STREAM is for TCP. 0 is for default protocol.
    if (sockfd < 0)
    {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void start_server(int sockfd)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error on binding");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, BACKLOG) < 0)
    {
        perror("Error on listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    char *server_ip = inet_ntoa(addr.sin_addr);
    printf("Server is listening on port %d. Ip: %s...\n", PORT, server_ip);
}

int get_client(int sockfd)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd < 0)
    {
        perror("Error on accept");
        close(sockfd);
        return -1;
    }

    char *client_ip = inet_ntoa(client_addr.sin_addr);

    printf("Client connected. With IP: %s\n", client_ip);

    return client_fd;
}

//=========================================================review=====================================================


void close_client(int client_fd)
{
    if (client_fd >= 0)
    {
        close(client_fd);
        printf("Client disconnected.\n");
    }
}

void close_server(int sockfd)
{
    if (sockfd >= 0)
    {
        close(sockfd);
        printf("Server socket closed.\n");
    }
}

void *thread_runner(void *arg)
{
    Queue *queue = (Queue *)arg;
    for (;;)
    {
        Task *task = dequeue(queue);

        if (task && task->task_function)
        {
            task->task_function(task->arg);
            destroy_task(task);
        }
    }
}

void thread_pool_init(Queue *queue)
{
    pthread_t threads[10];
    for (int i = 0; i < 10; i++)
    {
        if (pthread_create(&threads[i], NULL, thread_runner, (void *)queue) != 0)
        {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }
}

//====================================================================review===========================================

char* read_file(const char* filename, size_t* length){
    FILE* file = fopen(filename, "r");

    if(!file) return NULL;

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = malloc(*length + 1);
    fread(content, 1, *length, file);
    content[*length] = '\0';
    fclose(file);

    return content;
}