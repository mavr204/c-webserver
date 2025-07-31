#include "request_handler.h"

void request_handler(int client_fd, Queue *queue, void* (*function)(void*))
{
    int *fd_ptr = malloc(sizeof(int));
    *fd_ptr = client_fd;

    Task *task = malloc(sizeof(Task));
    task->client_fd = client_fd;
    task->task_function = function;
    task->arg = (void *)fd_ptr;

    enqueue(queue, task);
}

void *serve_home(void *arg)
{
    int client_fd = *(int *)arg;
    free(arg);

    size_t length;
    char *response = read_file("/home/mav204/Documents/programs/http-server/web/index.html", &length);

    if (!response)
    {
        perror("Failed to read index.html");
        close(client_fd);
        return NULL;
    }

    char header[256];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n\r\n",
             length);

    send(client_fd, header, strlen(header), 0);
    send(client_fd, response, length, 0);

    free(response);
    close(client_fd);

    return NULL;
}
