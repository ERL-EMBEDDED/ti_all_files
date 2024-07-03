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
    int server_port = 8888;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server\n");

    // Send Can_Data to the server
    int can_data;
    printf("Enter Can_Data: ");
    scanf("%d", &can_data);
    if (send(client_socket, &can_data, sizeof(can_data), 0) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    printf("Sent Can_Data to the server\n");

    // Close the socket
    close(client_socket);

    return 0;
}

