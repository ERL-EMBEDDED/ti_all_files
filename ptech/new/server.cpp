#include "server.h"

server::server() {
    actualVehicleSpeed = std::make_shared<float>(0.0f); // Initialize to 0
    expectedVehicleSpeed = std::make_shared<float>(0.0f); // Initialize to 0
}

server::~server() {
    closeSocketConnection();
}

void server::initSocketConnection() {
    etherNERIServer_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (etherNERIServer_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    etherNERIServer_address.sin_family = AF_INET;
    etherNERIServer_address.sin_addr.s_addr = INADDR_ANY;
    // Assuming etherNERIPort is defined elsewhere
    etherNERIServer_address.sin_port = htons(etherNERIPort);

    if (bind(etherNERIServer_socket, (struct sockaddr *)&etherNERIServer_address, sizeof(etherNERIServer_address)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(etherNERIServer_socket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "etherNERI Path Server Ready" << std::endl;

    while (true) {
        socklen_t etherNERIClient_address_len = sizeof(etherNERIClient_address);
        etherNERIClient_socket = accept(etherNERIServer_socket, (struct sockaddr *)&etherNERIClient_address, &etherNERIClient_address_len);
        if (etherNERIClient_socket == -1) {
            perror("Accept failed");
            usleep(1000);
            continue;
        }
        std::cout << "etherNERI Send Path: Got connection from " << inet_ntoa(etherNERIClient_address.sin_addr) << std::endl;
        break;
    }
}

void server::closeSocketConnection() {
    if (etherNERIClient_socket != -1) {
        close(etherNERIClient_socket);
    }
    if (etherNERIServer_socket != -1) {
        close(etherNERIServer_socket);
    }
}

void server::sendVehicleSpeed() {
    sendTcpipFrame[0] = 0x01; // Assuming CAN message ID
    memcpy(&sendTcpipFrame[1], actualVehicleSpeed.get(), sizeof(*actualVehicleSpeed));
    send(etherNERIClient_socket, sendTcpipFrame, TCPIP_FRAME_SIZE, 0);
}

void server::receiveVehicleSpeed() {
    recv(etherNERIClient_socket, receiveTcpipFrame, TCPIP_FRAME_SIZE, 0);
    memcpy(expectedVehicleSpeed.get(), &receiveTcpipFrame[1], sizeof(*expectedVehicleSpeed));
    std::cout << "Received Expected Speed: " << *expectedVehicleSpeed << std::endl;
}

void server::run() {
    while (true) {
        std::cout << "server Thread is running" << std::endl;
        receiveVehicleSpeed();
        sendVehicleSpeed();
    }
}

