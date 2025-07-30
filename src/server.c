#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#define PORT 8080
#define BACKLOG 5

int main() {
    int sockfd, newsockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket returns a file descriptor. AF_INET is for IPv4, SOCK_STREAM is for TCP. 0 is for default protocol.
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("Error on binding");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, BACKLOG) < 0){
        perror("Error on listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d...\n", PORT);

    for(;;){
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);

        if (client_fd < 0) {
            perror("Error on accept");
            close(sockfd);
            continue;
        }

        char *client_ip = inet_ntoa(client_addr.sin_addr);

        printf("Client connected. With IP: %s\n", client_ip);

        const char *http_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, world!";

        send(client_fd, http_response, strlen(http_response), 0);

        close(client_fd); 
    }
}