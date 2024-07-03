#ifndef TCPIP_CLIENT_H
#define TCPIP_CLIENT_H

struct ThreadArgs {
    const char *server_ip;
    int server_port;
    char *Tcpip_Receive_Message;
};

void *Tcpip_client_thread(void *arg);

#endif  // TCPIP_CLIENT_H

