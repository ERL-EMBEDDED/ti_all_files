#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    const char *server_ip = "192.168.3.12";
    int server_port = 8888;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_address.sin_addr);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Receive a message from the server
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("Error receiving data");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Received message from server: %s\n", buffer);

    // Check if the received message is "400"
    if (strcmp(buffer, "400") == 0) {
        // Send "200" to the server
        const char *response = "200";
        ssize_t bytes_sent = send(client_socket, response, strlen(response), 0);
        if (bytes_sent == -1) {
            perror("Error sending data");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        printf("Sent response to the server: %s\n", response);

        // Receive Can_Data from the server
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Error receiving data");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0';
        printf("Received Can_Data from server: %s\n", buffer);
    }

    // Close the connection
    close(client_socket);

    return EXIT_SUCCESS;
}

