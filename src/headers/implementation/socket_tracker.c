#include "socket_tracker.h"

ClientSession *create_node(int fd)
{
    ClientSession *client_session = (ClientSession *)malloc(sizeof(ClientSession));
    client_session->fd = fd;
    client_session->next = NULL;
    client_session->last_active = time(NULL);

    return client_session;
}

bool is_empty(ClientSession *head)
{
    return head == NULL;
}

void add_client(ClientSession **head_ref, int fd)
{
    ClientSession *new_node = create_node(fd);
    new_node->next = *head_ref;
    *head_ref = new_node;

    printf("New client %d added!\n", new_node->fd);
}

void delete_client(ClientSession **head_ref, void *target, bool is_node)
{
    if (*head_ref == NULL)
    {
        printf("No client to delete\n");
        return;
    }

    ClientSession *curr = *head_ref;
    ClientSession *prev = NULL;

    while (curr != NULL)
    {
        bool match = is_node ? (curr == (ClientSession *)target)
                             : (curr->fd == *(int *)target);

        if (match)
        {
            if (prev == NULL)
                *head_ref = curr->next;
            else
                prev->next = curr->next;

            printf("Deleted client with fd: %d\n", curr->fd);
            close(curr->fd);
            free(curr);
            return;
        }

        prev = curr;
        curr = curr->next;
    }

    printf("Target was not found!\n");
}

void validate(ClientSession **head_ref, ClientSession *target)
{
    if (difftime(time(NULL), target->last_active) >= TIMEOUT)
    {
        printf("Client %d timed out.\n", target->fd);
        delete_client(head_ref, target, true);
    }
}

void update_validity(ClientSession *target)
{
    target->last_active = time(NULL);
}

void free_all_clients(ClientSession **head_ref)
{
    ClientSession *curr = *head_ref;
    while (curr != NULL)
    {
        ClientSession *tmp = curr;
        curr = curr->next;
        close(tmp->fd);
        free(tmp);
    }
    *head_ref = NULL;
}

void print_clients(ClientSession *head) {
    printf("Connected clients:\n");
    while (head != NULL) {
        printf("  FD: %d | Last active: %ld\n", head->fd, head->last_active);
        head = head->next;
    }
}

