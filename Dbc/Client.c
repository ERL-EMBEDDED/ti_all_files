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
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Specify the IP address and port of the server
    const char *server_ip = "192.168.3.12";
    int server_port = 9999;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", server_ip, server_port);

    // Receive data from the server
    unsigned int received_data;
    ssize_t bytes_received = recv(client_socket, &received_data, sizeof(received_data), 0);
    if (bytes_received == -1) {
        perror("Receive failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    } else if (bytes_received == 0) {
        printf("Connection closed by server\n");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Received data from server: %u\n", received_data);
    
    // Perform conversion
    float converted_data = (received_data * 500) / 65536;

    // Send data to the server
    if (send(client_socket, &converted_data , sizeof(converted_data), 0) == -1) {
        perror("Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Sent CAN data to server: %f\n", converted_data);

    // Close the socket
    close(client_socket);

    return 0;
}

