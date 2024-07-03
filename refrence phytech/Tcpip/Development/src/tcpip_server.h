#ifndef TCPIP_SERVER_H
#define TCPIP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

struct ThreadData {
    int client_socket;
    struct sockaddr_in client_address;
};

void handle_client(void *arg);

#endif /* TCPIP_SERVER_H */

