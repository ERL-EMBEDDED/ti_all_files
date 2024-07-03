#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <memory>

#define TCPIP_FRAME_SIZE 8 // Assuming TCP/IP frame size

// Declaration of host and etherNERIPort as extern variables
extern const char *host;
extern int etherNERIPort;

class server {
private:
    int etherNERIServer_socket;
    int etherNERIClient_socket;
    struct sockaddr_in etherNERIServer_address;
    struct sockaddr_in etherNERIClient_address;
    unsigned char sendTcpipFrame[TCPIP_FRAME_SIZE];
    unsigned char receiveTcpipFrame[TCPIP_FRAME_SIZE];
    std::shared_ptr<float> actualVehicleSpeed;
    std::shared_ptr<float> expectedVehicleSpeed;
public:
    server();
    ~server();
    void initSocketConnection();
    void closeSocketConnection();
    void sendVehicleSpeed();
    void receiveVehicleSpeed();
    void run();
};

#endif // SERVER_H

