#include "epsControl.h"
#include <iostream>

// Constructor
epsControl::epsControl() {
    clientSocket = -1; // Initialize socket to an invalid value
}

// Destructor
epsControl::~epsControl() {
    if (clientSocket != -1) {
        close(clientSocket);
    }
}

// Method to initialize socket connection
void epsControl::initSocketConnection() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(clientSocket);
        clientSocket = -1;
    }
}

// Method to close socket connection
void epsControl::closeSocketConnection() {
    if (clientSocket != -1) {
        close(clientSocket);
        clientSocket = -1;
    }
}

// Method to send vehicle speed
void epsControl::sendVehicleSpeed() {
    // Example data to send
    char sendCANFrame[CAN_FRAME_SIZE] = {0};

    int bytesSent = send(clientSocket, sendCANFrame, CAN_FRAME_SIZE, 0);
    if (bytesSent == -1) {
        std::cerr << "Failed to send data" << std::endl;
    }
}

// Method to receive vehicle speed
void epsControl::receiveVehicleSpeed() {
    char receiveCANFrame[CAN_FRAME_SIZE] = {0};

    int bytesReceived = recv(clientSocket, receiveCANFrame, CAN_FRAME_SIZE, 0);
    if (bytesReceived == -1) {
        std::cerr << "Failed to receive data" << std::endl;
    }
}

