#include "tcpip_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

void *Tcpip_client_thread(void *arg) {
    struct tcp_ThreadArgs *tcp_threadArgs = (struct tcp_ThreadArgs *)arg;

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(tcp_threadArgs->server_port);
    inet_pton(AF_INET, tcp_threadArgs->server_ip, &server_address.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("Error receiving data");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Received message from server: %s\n", buffer);

    if (strcmp(buffer, "400") == 0) {
        const char *response = "200";
        ssize_t bytes_sent = send(client_socket, response, strlen(response), 0);
        if (bytes_sent == -1) {
            perror("Error sending data");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        printf("Sent response to the server: %s\n", response);

        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Error receiving data");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0';
        strcpy(tcp_threadArgs->Tcpip_Receive_Message, buffer);
        // printf("Received Can_Data from server: %s\n", buffer);
    }

    close(client_socket);

    return NULL;
}

void runTcpipClient(const char *server_ip, int server_port, char *Tcpip_Receive_Message) {
    pthread_t tcpip_thread;

    // Create a structure to hold the thread arguments
    struct tcp_ThreadArgs tcp_threadArgs;
    tcp_threadArgs.server_ip = server_ip;
    tcp_threadArgs.server_port = server_port;

    // Call the TCP/IP client function
    pthread_create(&tcpip_thread, NULL, Tcpip_client_thread, (void *)&tcp_threadArgs);

    // Main thread continues with other tasks or waits for completion

    // Join the TCP/IP thread
    pthread_join(tcpip_thread, NULL);

    // Copy the received message back to the provided buffer
    strcpy(Tcpip_Receive_Message, tcp_threadArgs.Tcpip_Receive_Message);
}

