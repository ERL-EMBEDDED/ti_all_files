#include "tcpip_server.h"
#include <pthread.h>

int main() {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Specify the IP address and port
    const char *host = "192.168.3.12";
    int port = 1234;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(host);
    server_address.sin_port = htons(port);

    // Bind to the IP address and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", host, port);

    while (1) {
        // Wait for a connection
        int client_socket;
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);

        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        // Create a thread to handle the client
        pthread_t thread;
        struct ThreadData *thread_data = (struct ThreadData *)malloc(sizeof(struct ThreadData));
        thread_data->client_socket = client_socket;
        thread_data->client_address = client_address;

        if (pthread_create(&thread, NULL, (void *)&handle_client, (void *)thread_data) != 0) {
            perror("Error creating thread");
            close(client_socket);
            free(thread_data);
        }
    }

    // Close the server socket
    close(server_socket);

    return 0;
}

