#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define CAN_FRAME_SIZE 5
#define SERVER_PORT 8080 // Example port; change as necessary
#define SERVER_IP "192.168.3.12" // Example IP; change as necessary

class client {
public:
    int clientSocket;
    struct sockaddr_in serverAddr;

    client();
    ~client();
    void initSocketConnection();
    void closeSocketConnection();
    void sendVehicleSpeed(unsigned int expectedVehicleSpeed);
    void receiveVehicleSpeed();
};

#endif // CLIENT_H

