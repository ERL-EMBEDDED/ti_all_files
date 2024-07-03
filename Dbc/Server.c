#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

// Function to get current time with milliseconds
void current_time(char *buffer) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *tm_info = localtime(&tv.tv_sec);
    sprintf(buffer, "%02d:%02d:%02d:%03ld", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, tv.tv_usec / 1000);
}

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
        close(server_socket); // Close the socket before exiting
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        close(server_socket); // Close the socket before exiting
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", host, port);

    while (1) {
        // Prompt the user for CAN data
        unsigned int can_data;
        printf("Enter CAN data for VSP: ");
        scanf("%u", &can_data);

        // Wait for a connection
        int client_socket;
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);

        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        char timestamp[20]; // Buffer for storing the timestamp
        current_time(timestamp);
        printf("[%s] Got connection from %s\n", timestamp, inet_ntoa(client_address.sin_addr));

        // Send the CAN data to the client
        if (send(client_socket, &can_data, sizeof(can_data), 0) == -1) {
            perror("Send failed");
            close(client_socket);
            continue;
        }
        
        current_time(timestamp);
        printf("[%s] Sent CAN data to the client: %u\n", timestamp, can_data);

        // Receive data from the client
        unsigned int received_data;
        ssize_t bytes_received = recv(client_socket, &received_data, sizeof(received_data), 0);
        if (bytes_received == -1) {
            perror("Receive failed");
            close(client_socket);
            continue;
        }

        // Print received data
        current_time(timestamp);
        printf("[%s] Received data from the client: %u\n", timestamp, received_data);

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket (this part will never be reached in this code)
    close(server_socket);

    return 0;
}

