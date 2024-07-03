#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

void printTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t rawtime = tv.tv_sec;
    struct tm *timeinfo;
    char buffer[80];

    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
    printf("%s.%03ld - ", buffer, tv.tv_usec / 1000);
}

int main() {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

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

    while (1) {
        // Wait for a connection
        int client_socket;
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);

        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) == -1) {
            perror("Accept failed");
            continue;
        }

        // Print current time with milliseconds
        printTime();

        printf("Got connection from %s\n", inet_ntoa(client_address.sin_addr));

        // Send a message to the client
        const char *message = "400";
        send(client_socket, message, strlen(message), 0);

        // Receive data from the client
        char buffer[1024];
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Error receiving data");
            close(client_socket);
            continue;
        }

        buffer[bytes_received] = '\0';

        // Print current time with milliseconds
        printTime();

        printf("Received message: %s\n", buffer);

        if (strcmp(buffer, "200") == 0) {
            // Print current time with milliseconds
            printTime();

            printf("Verification Success\n");

            // Send Can_Data to the client
            const char *can_data = "123#000400";
            send(client_socket, can_data, strlen(can_data), 0);

            // Print current time with milliseconds
            printTime();

            printf("Sent Can_Data to the client\n");
        }

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}

