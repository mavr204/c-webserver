#include "request_handler.h"

void request_handler(int client_fd, Queue *queue)
{
    Context *ctx = malloc(sizeof(Context));
    ctx->fd = client_fd;
    ctx->page = route_table[1].page;

    Task *task = malloc(sizeof(Task));
    task->client_fd = client_fd;
    task->task_function = serve_page;
    task->arg = (void *)ctx;

    enqueue(queue, task);
}

// void request_handler(int client_fd, Queue *queue)
// {
//     Context *ctx = malloc(sizeof(Context));
//     ctx->fd = client_fd;

    
// }

void *serve_page(void *arg)
{   
    Context *ctx = (Context*)arg;
    int client_fd = ctx->fd;
    const char *PAGE = ctx->page;


    size_t length;
    char *response = read_file(PAGE, &length);

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