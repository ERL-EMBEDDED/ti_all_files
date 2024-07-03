#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "global.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999
#define TCPIP_FRAME_SIZE 5

class ClientTcpip {
private:
    int clientSocket;
    struct sockaddr_in serverAddr;
    char sendTcpipFrame[TCPIP_FRAME_SIZE];
    char receiveTcpipFrame[TCPIP_FRAME_SIZE];

public:
    ClientTcpip() {
        clientSocket = -1; // Initialize socket to an invalid value
    }

    ~ClientTcpip() {
        if (clientSocket != -1)
            close(clientSocket);
    }

    bool connectServer() {
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

    void sendVehicleSpeed() {
        // Send vehicle speed
        int ActualSpeed; // Example speed
        printf("Enter ActualSpeed\n");
        scanf("%d", &ActualSpeed);
        sendTcpipFrame[0] = 0x01; // Assuming CAN message ID
        memcpy(&sendTcpipFrame[1], &ActualSpeed, sizeof(ActualSpeed));
        send(clientSocket, sendTcpipFrame, TCPIP_FRAME_SIZE, 0);
    }

    void receiveVehicleSpeed() {
        // Receive encoded message from server
        recv(clientSocket, receiveTcpipFrame, TCPIP_FRAME_SIZE, 0);

        // Decode received message
        memcpy(*expectedVehicleSpeed, &receiveTcpipFrame[1], sizeof(receiveTcpipFrame));
        std::cout << "Received Expectedspeed: " << *expectedVehicleSpeed << std::endl;
    }

    bool isConnected() const {
        return clientSocket != -1;
    }

    void disconnect() {
        // Close the client socket
        if (clientSocket != -1) {
            close(clientSocket);
            clientSocket = -1;
        }
    }
};

int main() {
    ClientTcpip tcpip;
    if (tcpip.connectServer()) { // Check if connection was successful
        while (true) {
            tcpip.receiveVehicleSpeed();
            tcpip.sendVehicleSpeed();
        }
        tcpip.disconnect(); // Disconnect after exiting the loop
    }
    return 0;
}


