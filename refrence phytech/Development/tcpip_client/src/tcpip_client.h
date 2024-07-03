#ifndef TCPIP_CLIENT_H
#define TCPIP_CLIENT_H

#include <pthread.h>

struct ThreadArgs {
    const char *server_ip;
    int server_port;
    char Tcpip_Receive_Message[1024];
};

void runTcpipClient(const char *server_ip, int server_port, char *Tcpip_Receive_Message);

#endif  // TCPIP_CLIENT_H

