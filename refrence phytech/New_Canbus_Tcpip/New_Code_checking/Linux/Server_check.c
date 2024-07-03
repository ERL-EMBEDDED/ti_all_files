#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#define MAX_CLIENTS 5
#define MAX_MESSAGE_SIZE 1024

void *handle_client(void *arg);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t thread_id;

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.3.12");
    server_addr.sin_port = htons(142);

    // Bind the server socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding server socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 888\n");

    while (1) {
        // Accept a new connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) != 0) {
            perror("Error creating thread");
            close(client_socket);
        }
    }

    close(server_socket);

    return 0;
}

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char message[MAX_MESSAGE_SIZE];

    while (1) {
        // Receive data from the client
        ssize_t recv_size = recv(client_socket, message, sizeof(message), 0);
        if (recv_size <= 0) {
            printf("Connection closed\n");
            break;
        }

        // Null-terminate the received data
        message[recv_size] = '\0';

        // Get current time with milliseconds
        struct timeval tv;
        gettimeofday(&tv, NULL);
        time_t current_time = tv.tv_sec;
        int milliseconds = tv.tv_usec / 1000;

        // Calculate hours, minutes, and seconds using localtime
        struct tm *tm_info;
        tm_info = localtime(&current_time);

        // Print data with timestamp in the format hr:min:sec:millisec
        printf("[%02d:%02d:%02d:%03d] Received data from client: %s\n",
               tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, milliseconds, message);
        // Add your processing logic here

        // You can also send a response back to the client if needed
        // send(client_socket, response_message, strlen(response_message), 0);
    }

    close(client_socket);
    pthread_exit(NULL);
}

