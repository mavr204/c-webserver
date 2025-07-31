#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "safe_queue.h"

void* serve_home(void* arg);
void request_handler(int client_fd, Queue *queue);

#endif // REQUEST_HANDLER_H
