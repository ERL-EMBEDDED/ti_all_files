#include "client.h"

ClientTcpip::ClientTcpip() {
    clientSocket = -1; // Initialize socket to an invalid value
}

ClientTcpip::~ClientTcpip() {
    if (clientSocket != -1)
        close(clientSocket);
}

bool ClientTcpip::connectToServer() {
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

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed\n";
        close(clientSocket);
        clientSocket = -1;
        return false;
    }

    return true;
}

void ClientTcpip::sendVehicleSpeed(float speed) {
    // Send vehicle speed
    sendTcpipFrame[0] = 0x01; // Assuming CAN message ID
    memcpy(&sendTcpipFrame[1], &speed, sizeof(speed));
    send(clientSocket, sendTcpipFrame, TCPIP_FRAME_SIZE, 0);
}

void ClientTcpip::receiveVehicleSpeed() {
    // Receive encoded message from server
    recv(clientSocket, receiveTcpipFrame, TCPIP_FRAME_SIZE, 0);

    // Decode received message and store it in a variable
    float receivedSpeed;
    memcpy(&receivedSpeed, &receiveTcpipFrame[1], sizeof(receivedSpeed));
    std::cout << "Received Speed: " << receivedSpeed << std::endl;
}

void ClientTcpip::disconnectFromServer() {
    // Close the client socket
    if (clientSocket != -1) {
        close(clientSocket);
        clientSocket = -1;
    }

