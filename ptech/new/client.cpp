#include "client.h"

ClientTcpip::ClientTcpip() {
    clientSocket = -1; // Initialize socket to an invalid value
}

ClientTcpip::~ClientTcpip() {
    if (clientSocket != -1)
        close(clientSocket);
}

bool ClientTcpip::connectServer() {
    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error creating socket\n";
        return false;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed\n";
        close(clientSocket);
        clientSocket = -1;
        return false;
    }
    return true;
}

void ClientTcpip::messageEncoding() {
    memset(sendCANFrame, 0, 5);
    sendCANFrame[0] = 1; // Example value; change as necessary
    unsigned int *intDataptr = reinterpret_cast<unsigned int*>(&expectedVehicleSpeed);
    unsigned char *charDataptr = reinterpret_cast<unsigned char*>(intDataptr);
    for(int i=0; i<4; i++) {
        sendCANFrame[i+1] = *(charDataptr+i);
        std::cout << "charDataptr[" << i << "]:[" << std::hex << static_cast<int>(*(charDataptr+i)) << "]" << std::endl;
    }
    for(int i=0; i<5; i++) {
        std::cout << "sendCAN[" << i << "] : [" << std::hex << static_cast<int>(sendCANFrame[i]) << "]" << std::endl;
    }
}

void ClientTcpip::sendVehicleSpeed() {
    int bytesSent = send(clientSocket, sendCANFrame, CAN_FRAME_SIZE, 0);
    if (bytesSent == CAN_FRAME_SIZE) {
        std::cout << "Vehicle speed data sent successfully." << std::endl;
    } else if (bytesSent == -1) {
        std::cerr << "Error in sending data." << std::endl;
    } else {
        std::cerr << "Unexpected behavior in sending data. Sent bytes: " << bytesSent << std::endl;
    }
}

void ClientTcpip::messageDecoding() {
    unsigned int vehicleSpeed = 0; // Declare vehicleSpeed within the function
    for(int i = 0; i < CAN_FRAME_SIZE - 1; i++) { // Use CAN_FRAME_SIZE instead of CAN_DATA_SIZE
        receiveCANFrame[i] = receiveCANFrame[i+1];
    }

    // Decode the vehicle speed from receiveCANFrame
    vehicleSpeed = vehicleSpeed | ((0xFFFF00 | (receiveCANFrame[3])) << 24);
    vehicleSpeed = vehicleSpeed | ((0xFFFF00 | (receiveCANFrame[2])) << 16);
    vehicleSpeed = vehicleSpeed | ((0xFF00 | (receiveCANFrame[1])) << 8);
    vehicleSpeed = vehicleSpeed | (0xFF & (receiveCANFrame[0]));

    std::cout << "Decoded Vehicle Speed: " << vehicleSpeed << std::endl;
}

void ClientTcpip::receiveVehicleSpeed() {
    int bytesReceived = recv(clientSocket, receiveCANFrame, CAN_FRAME_SIZE, 0);
    if (bytesReceived > 0) {
        int receivedSpeed;
        memcpy(&receivedSpeed, &receiveCANFrame[1], sizeof(receivedSpeed));
        std::cout << "Received Vehicle Speed: " << receivedSpeed << std::endl;
    } else if (bytesReceived == 0) {
        std::cout << "No data received within " << std::endl;
    } else {
        std::cerr << "Error in receiving data." << std::endl;
    }
}

void ClientTcpip::closeSocketConnection() {
    if (clientSocket != -1) {
        close(clientSocket);
        clientSocket = -1;
    }
}
