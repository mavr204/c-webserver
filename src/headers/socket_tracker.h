#ifndef socket_tracker
#define socket_tracker
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <netinet/in.h>

#define TIMEOUT (30 * 60)

typedef struct ClientSession
{
    int fd;
    time_t last_active;
    ClientSession *next;
} ClientSession;

ClientSession *create_node(int fd);
bool is_empty(ClientSession *head);
void add_client(ClientSession **head_ref, int fd);
void delete_client(ClientSession **head_ref, void *target, bool is_node);
void validate(ClientSession **head_ref, ClientSession *target);
void update_validity(ClientSession *target);
void free_all_clients(ClientSession **head_ref);
void print_clients(ClientSession *head);

#endif