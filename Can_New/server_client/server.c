#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Specify the IP address and port
    const char *host = "192.168.3.12";
    int port = 9999;

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

    int client_socket;
    struct sockaddr_in client_address;
    socklen_t client_address_len;

    while (1) {
        // Wait for a connection
        client_address_len = sizeof(client_address);
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Got connection from %s\n", inet_ntoa(client_address.sin_addr));
        int can_data;
        scanf("%d", &can_data);

        // Send Can_Data to the client
        if (send(client_socket, &can_data, sizeof(can_data), 0) == -1) {
            perror("Send failed");
        } else {
            printf("Sent Can_Data to the client\n");
        }

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket (This part is never reached)
    close(server_socket);

    return 0;
}

