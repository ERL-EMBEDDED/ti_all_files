#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define CAN_FRAME_SIZE 5
#define SERVER_PORT 8888 // Example port; change as necessary
#define SERVER_IP "192.168.3.12" // Example IP; change as necessary

class ClientTcpip {
public:
    int clientSocket;
    struct sockaddr_in serverAddr;
    unsigned char sendCANFrame[CAN_FRAME_SIZE];
    unsigned char receiveCANFrame[CAN_FRAME_SIZE];
    unsigned int expectedVehicleSpeed;
    unsigned int actualVehicleSpeed;

    ClientTcpip();
    ~ClientTcpip();
    bool connectServer();
    void messageEncoding();
    void sendVehicleSpeed();
    void messageDecoding();
    void receiveVehicleSpeed();
    void closeSocketConnection();
};

#endif // CLIENT_H

