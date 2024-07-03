#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "192.168.0.117"
#define SERVER_PORT 8800
#define BACKLOG 5
#define TCPIP_FRAME_SIZE 5

class epsServer {
private:
    int serverSocket;
    int clientSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    socklen_t addrLen;
    int vehicleSpeed;

public:
    epsServer();
    ~epsServer();
    void initSocket();
    int acceptClient();
    void handleClient();
    void closeClientSocket();
};

class ClientTcpip {
private:
    int clientSocket;
    char sendTcpipFrame[TCPIP_FRAME_SIZE];
    char receiveTcpipFrame[TCPIP_FRAME_SIZE];
    int* expectedVehicleSpeed;
    int* actualVehicleSpeed;

public:
    ClientTcpip(int socket);
    ~ClientTcpip();
    void sendVehicleSpeed();
    int receiveVehicleSpeed();
};

#endif /* SERVER_H */

