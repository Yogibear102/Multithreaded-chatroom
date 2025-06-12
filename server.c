#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(char *message, int sender_fd)
{
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i] != 0 && clients[i] != sender_fd)
        {
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg)
{
    int client_fd = *((int *)arg);
    free(arg);

    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("Received: %s", buffer);
        broadcast_message(buffer, client_fd);
    }

    // Remove client on disconnect
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i] == client_fd)
        {
            clients[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    close(client_fd);
    printf("Client disconnected\n");
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);

    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on 127.0.0.1:%d\n", port);

    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len);
        if (new_socket < 0)
        {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&clients_mutex);
        int i;
        for (i = 0; i < MAX_CLIENTS; ++i)
        {
            if (clients[i] == 0)
            {
                clients[i] = new_socket;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        if (i == MAX_CLIENTS)
        {
            printf("Max clients reached. Connection refused.\n");
            close(new_socket);
        }
        else
        {
            printf("New client connected\n");
            pthread_t tid;
            int *pclient = malloc(sizeof(int));
            *pclient = new_socket;
            pthread_create(&tid, NULL, &handle_client, pclient);
            pthread_detach(tid);
        }
    }

    close(server_fd);
    return 0;
}
