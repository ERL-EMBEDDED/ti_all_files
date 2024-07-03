#include "server.h"

epsServer::epsServer() {
    serverSocket = -1; 
    clientSocket = -1;
    vehicleSpeed = 0;
    addrLen = sizeof(clientAddr);
}

epsServer::~epsServer() {
    if (clientSocket != -1) {
        close(clientSocket);
    }
    if (serverSocket != -1) {
        close(serverSocket);
    }
}

void epsServer::initSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(serverSocket);
        serverSocket = -1;
        return;
    }

    if (listen(serverSocket, BACKLOG) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(serverSocket);
        serverSocket = -1;
    }
}

int epsServer::acceptClient() {
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept client" << std::endl;
        return 1;
    }
    else {
          std::cerr << " accept client" << std::endl;
         return 0;
    }
}

void epsServer::handleClient() {
    while (true) {
        ClientTcpip client(clientSocket); // Create a client object
        client.receiveVehicleSpeed();
        client.sendVehicleSpeed();
    }
}

void epsServer::closeClientSocket() {
    if (clientSocket != -1) {
        close(clientSocket);
        clientSocket = -1;
    }
}

ClientTcpip::ClientTcpip(int socket) : clientSocket(socket) {
    expectedVehicleSpeed = new int;
    actualVehicleSpeed = new int;
}

ClientTcpip::~ClientTcpip() {
    delete expectedVehicleSpeed;
    delete actualVehicleSpeed;
}

void ClientTcpip::sendVehicleSpeed() {
    sendTcpipFrame[0] = 0x01; // Assuming CAN message ID
    memcpy(&sendTcpipFrame[1], actualVehicleSpeed, sizeof(*actualVehicleSpeed));
    send(clientSocket, sendTcpipFrame, TCPIP_FRAME_SIZE, 0);
    std::cout << "send Autualspeed: " << *actualVehicleSpeed << std::endl;
}

int ClientTcpip::receiveVehicleSpeed() {
    int byteRead=recv(clientSocket, receiveTcpipFrame, TCPIP_FRAME_SIZE, 0);
    if(byteRead >0){
         memcpy(expectedVehicleSpeed, &receiveTcpipFrame[1], sizeof(*expectedVehicleSpeed));
         std::cout << "Received Expectedspeed: " << *expectedVehicleSpeed << std::endl;
         return 0;
         }
         else  {
               std::cout << "Not Received Expectedspeed " << std::endl;
               return 1;
               
         }
}

int main() {
    epsServer server;
    server.initSocket();
    std::cout << "Server initialized and waiting for connections..." << std::endl;

    while (true) {
       // int connection= server.acceptClient();
        //if(connection <0)
        //{
        server.acceptClient();
        std::cout << "Client connected, handling client..." << std::endl;
        server.handleClient();
        server.closeClientSocket(); // Close client socket after handling
       // }
       // else {
          // std::cout << "Client not connected..." << std::endl;
          // server.acceptClient();
        //}
    }

    return 0;
}
