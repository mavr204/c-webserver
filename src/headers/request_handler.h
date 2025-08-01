#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "safe_queue.h"

#define PAYLOAD_BUFFER_SIZE (8*1024)

void* serve_page(void* arg);
void request_handler(int client_fd, Queue *queue);
char* read_file(const char* filename, size_t* length);
const char *get_page(char *method, char *path);

#define HOME_PAGE "/home/mav204/Documents/programs/http-server/web/index.html"
#define PAGE_404 "/home/mav204/Documents/programs/http-server/web/404.html"

typedef struct Context {
    int fd;
    const char *page;
} Context;

typedef struct Route {
    const char *method;
    const char *path;
    const char *page;
} Route;

//==================================================================To-Do:Switch Array with BST==================================================================
static Route route_table[] = {
    { "GET", "/", HOME_PAGE },
    { NULL, NULL, PAGE_404 }
};


#endif 
