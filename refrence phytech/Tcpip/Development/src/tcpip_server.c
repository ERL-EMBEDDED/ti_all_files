#include "tcpip_server.h"

void handle_client(void *arg) {
    struct ThreadData *thread_data = (struct ThreadData *)arg;
    int client_socket = thread_data->client_socket;
    struct sockaddr_in client_address = thread_data->client_address;

    printf("Got connection from %s\n", inet_ntoa(client_address.sin_addr));

    // Send a message to the client
    const char *message = "400";
    send(client_socket, message, strlen(message), 0);

    // Receive data from the client
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        perror("Error receiving data");
        close(client_socket);
        free(arg);
        pthread_exit(NULL);
    }

    buffer[bytes_received] = '\0';
    printf("Received message: %s\n", buffer);

    if (strcmp(buffer, "200") == 0) {
        printf("Verification Success\n");

        // Send Can_Data to the client
        const char *can_data = "123#000400";
        send(client_socket, can_data, strlen(can_data), 0);
        printf("Sent Can_Data to the client\n");
    }

    // Close the client socket
    close(client_socket);
    free(arg);
    pthread_exit(NULL);
}

