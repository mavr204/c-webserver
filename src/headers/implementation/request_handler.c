#include "request_handler.h"

void request_handler(int client_fd, Queue *queue)
{
    char buffer[PAYLOAD_BUFFER_SIZE];
    int payload_size = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (payload_size > 0)
    {
        buffer[payload_size] = '\0';
    }
    else if (payload_size == 0)
    {
        printf("Client disconnected\n");
        close(client_fd);
        return;
    }
    else
    {
        perror("recv failed");
        close(client_fd);
        return;
    }

    char method[8], path[1024];
    sscanf(buffer, "%s %s", method, path);

    Context *ctx = malloc(sizeof(Context));
    if (!ctx)
    {
        printf("malloc has failed, exiting task!");
        close(client_fd);
        return;
    }
    ctx->fd = client_fd;
    ctx->page = get_page(method, path);

    Task *task = malloc(sizeof(Task));
    if (!task)
    {
        printf("malloc has failed, exiting task!");
        close(client_fd);
        return;
    }
    task->client_fd = client_fd;
    task->task_function = serve_page;
    task->arg = (void *)ctx;

    enqueue(queue, task);
}

const char *get_page(char *method, char *path)
{
    int route_table_size = sizeof(route_table) / sizeof(Route);
    printf("\nroute table size: %d\n", route_table_size);

    for (int i = 0; i < route_table_size && (route_table[i].method != NULL || route_table[i].path != NULL); i++)
    {
        if (strcmp(method, route_table[i].method) == 0 && strcmp(path, route_table[i].path) == 0)
            return route_table[i].page;
    }
    printf("404: %s", PAGE_404);
    return PAGE_404;
}

void *serve_page(void *arg)
{
    Context *ctx = (Context *)arg;
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

char *read_file(const char *filename, size_t *length)
{
    FILE *file = fopen(filename, "r");

    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(*length + 1);
    fread(content, 1, *length, file);
    content[*length] = '\0';
    fclose(file);

    return content;
}