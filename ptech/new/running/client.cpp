#include "client.h"
#include <cerrno>
#include <cstring>
#include <iostream>

// Constructor
client::client() {
    clientSocket = -1; // Initialize socket to an invalid value
}

// Destructor
client::~client() {
    closeSocketConnection(); // Ensure the socket is closed
}

// Method to initialize socket connection
void client::initSocketConnection() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    std::cout << "Attempting to connect to " << SERVER_IP << ":" << SERVER_PORT << std::endl;

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed: " << strerror(errno) << std::endl;
        close(clientSocket);
        clientSocket = -1;
    } else {
        std::cout << "Successfully connected to the server!" << std::endl;
    }
}

// Method to close socket connection
void client::closeSocketConnection() {
    if (clientSocket != -1) {
        close(clientSocket);
        clientSocket = -1;
    }
}

// Method to send vehicle speed
void client::sendVehicleSpeed(unsigned int expectedVehicleSpeed) {
    if (clientSocket == -1) {
        std::cerr << "Socket is not connected." << std::endl;
        return;
    }

    unsigned char sendCANFrame[CAN_FRAME_SIZE];
    memset(sendCANFrame, 0, CAN_FRAME_SIZE);

    // Encode the vehicle speed data
    sendCANFrame[0] = 0x55; // CAN message ID
    memcpy(&sendCANFrame[1], &expectedVehicleSpeed, sizeof(expectedVehicleSpeed));

    // Send the data
    int bytesSent = send(clientSocket, sendCANFrame, CAN_FRAME_SIZE, 0);
    if (bytesSent == CAN_FRAME_SIZE) {
        std::cout << "Vehicle speed data sent successfully." << std::endl;
    } else if (bytesSent == -1) {
        std::cerr << "Error in sending data: " << strerror(errno) << std::endl;
    } else {
        std::cerr << "Unexpected behavior in sending data. Sent bytes: " << bytesSent << std::endl;
    }
}

// Method to receive vehicle speed
void client::receiveVehicleSpeed() {
    if (clientSocket == -1) {
        std::cerr << "Socket is not connected." << std::endl;
        return;
    }

    char receiveCANFrame[CAN_FRAME_SIZE];
    int bytesReceived = recv(clientSocket, receiveCANFrame, CAN_FRAME_SIZE, 0);
    if (bytesReceived > 0) {
        unsigned int actualVehicleSpeed;
        memcpy(&actualVehicleSpeed, &receiveCANFrame[1], sizeof(actualVehicleSpeed));
        std::cout << "Received Vehicle Speed: " << actualVehicleSpeed << std::endl;
    } else if (bytesReceived == 0) {
        std::cout << "No data received." << std::endl;
    } else {
        std::cerr << "Error in receiving data: " << strerror(errno) << std::endl;
    }
}

int main() {
    client myClient;
    myClient.initSocketConnection();

    if (myClient.clientSocket != -1) {
        // Example logic for sending and receiving vehicle speed
        for (unsigned int expectedVehicleSpeed = 100; expectedVehicleSpeed < 500; expectedVehicleSpeed += 50) {
            myClient.sendVehicleSpeed(expectedVehicleSpeed);
            myClient.receiveVehicleSpeed();
        }

        myClient.closeSocketConnection();
    } else {
        std::cerr << "Failed to connect to server. Exiting." << std::endl;
    }

    return 0;
}

