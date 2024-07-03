#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Declarations for socket functions
extern int socket(int domain, int type, int protocol);
extern int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
extern ssize_t send(int sockfd, const void *buf, size_t len, int flags);
extern ssize_t recv(int sockfd, void *buf, size_t len, int flags);
extern int close(int sockfd);

// Declarations for network functions and structures
extern int inet_pton(int af, const char *src, void *dst);
extern unsigned short htons(unsigned short hostshort);

// Define the sockaddr_in structure if it's not already defined
struct sockaddr_in {
    short            sin_family;
    unsigned short   sin_port;
    struct in_addr   sin_addr;
    char             sin_zero[8];
};

// Define the in_addr structure if it's not already defined
struct in_addr {
    unsigned long s_addr;
};

#define SERVER_IP "127.0.0.1" // IP address of the server
#define SERVER_PORT 12345     // Port number on which server is listening
#define BUFFER_SIZE 1024      // Size of the buffer for sending and receiving data

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 address from text to binary form
    struct in_addr server_ip;
    if (inet_pton(AF_INET, SERVER_IP, &server_ip) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_addr = server_ip;

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send data to the server
    const char *message = "Hello, server!";
    if (send(sockfd, message, strlen(message), 0) != strlen(message)) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    printf("Message sent to server: %s\n", message);

    // Receive response from the server
    ssize_t bytes_received;
    if ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0)) < 0) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    printf("Server response: %s\n", buffer);

    // Close the socket
    close(sockfd);

    return 0;
}

