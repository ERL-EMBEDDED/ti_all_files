#ifndef SERVER_H
#define SERVER_H

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "globals.h"

#define CAN_FRAME_SIZE 5
#define CAN_DATA_SIZE 4

class server {
private:
    const char *host = "192.168.3.12"; // Optional for future use
    int etherNERIPort = 8080;
    int receivePort = 1111;

    int etherNERIServer_socket = -1;
    int etherNERIClient_socket = -1;
    int receiveServer_socket = -1;
    int receiveClient_socket = -1;

    struct sockaddr_in etherNERIServer_address {};
    struct sockaddr_in receiveServer_address {};
    struct sockaddr_in etherNERIClient_address {};
    struct sockaddr_in receiveClient_address {};

    unsigned int vehicleSpeed;
    unsigned int *intDataptr;
    unsigned char *charDataptr;

    unsigned char sendCANFrame[CAN_FRAME_SIZE];
    unsigned char receiveCANFrame[CAN_FRAME_SIZE];
    unsigned char sendCANData[CAN_DATA_SIZE];
    unsigned char receiveCANData[CAN_DATA_SIZE];
    unsigned char CANMsgId = 0x55;

    int bytes_received;

public:
    server();
    ~server();
    void initSocketConnection();
    void closeSocketConnection();
    void sendVehicleSpeed();
    void messageEncoding();
    void messageDecoding();
    void receiveVehicleSpeed();
    void epsControlThreadfunc();
};

#endif // SERVER_H

