#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_MESSAGE_SIZE 1024

void *receive_messages(void *arg);

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    pthread_t recv_thread;

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.3.12");
    server_addr.sin_port = htons(142);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Create a thread to receive messages
    if (pthread_create(&recv_thread, NULL, receive_messages, (void *)&client_socket) != 0) {
        perror("Error creating receive thread");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Send incrementing values to the server
    for (int i = 1; i >= 0; ++i) {
        char message[MAX_MESSAGE_SIZE];
        sprintf(message, "%d", i);

        // Send data to the server
        send(client_socket, message, strlen(message), 0);

        usleep(100);
    }

    // Wait for the receive thread to finish
    pthread_join(recv_thread, NULL);

    close(client_socket);

    return 0;
}

void *receive_messages(void *arg) {
    int client_socket = *((int *)arg);
    char message[MAX_MESSAGE_SIZE];

    while (1) {
        // Receive data from the server
        ssize_t recv_size = recv(client_socket, message, sizeof(message), 0);
        if (recv_size <= 0) {
            printf("Server closed the connection\n");
            break;
        }

        // Null-terminate the received data
        message[recv_size] = '\0';

        printf("Received data from server: %s\n", message);
        // Add your processing logic here
    }

    pthread_exit(NULL);
}

