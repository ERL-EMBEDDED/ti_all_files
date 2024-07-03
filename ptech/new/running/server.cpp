#include "server.h"
#include <time.h>

struct timespec next_time_;

server::server() {
    clock_gettime(CLOCK_MONOTONIC, &next_time_);
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

    // Specify the IP address and port
    etherNERIServer_address.sin_family = AF_INET;
    etherNERIServer_address.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
    etherNERIServer_address.sin_port = htons(etherNERIPort);

    // Bind to the IP address and port
    if (bind(etherNERIServer_socket, (struct sockaddr *)&etherNERIServer_address, sizeof(etherNERIServer_address)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(etherNERIServer_socket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("etherNERI Path Server Ready\n");

    while (true) {
        socklen_t etherNERIClient_address_len = sizeof(etherNERIClient_address);
        etherNERIClient_socket = accept(etherNERIServer_socket, (struct sockaddr *)&etherNERIClient_address, &etherNERIClient_address_len);
        if (etherNERIClient_socket == -1) {
            perror("Accept failed");
            usleep(10000);
            continue;
        }
        printf("etherNERI Send Path : Got connection from %s\n", inet_ntoa(etherNERIClient_address.sin_addr));
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

void server::messageEncoding() {
    memset(sendCANFrame, 0, CAN_FRAME_SIZE);
    sendCANFrame[0] = CANMsgId;
    vehicleSpeed = expectedVehicleSpeed;

    intDataptr = &vehicleSpeed;
    charDataptr = (unsigned char*)intDataptr;
    for (int i = 0; i < 4; i++) {
        sendCANFrame[i + 1] = *(charDataptr + i);
        printf("charDataptr[%d]:[%x] \n", i, *(charDataptr + i));
    }
    for (int i = 0; i < CAN_FRAME_SIZE; i++) {
        printf("sendCAN[%d] : [%x]\n", i, sendCANFrame[i]);
    }
}

void server::sendVehicleSpeed() {
    clock_gettime(CLOCK_MONOTONIC, &next_time_);
    printf("Before send timespec.tv_nsec:%jd.%09ld\n", next_time_.tv_sec, next_time_.tv_nsec);

    int bytesSent = send(etherNERIClient_socket, sendCANFrame, CAN_FRAME_SIZE, 0);
    printf("After Send timespec.tv_nsec:%jd.%09ld\n", next_time_.tv_sec, next_time_.tv_nsec);

    if (bytesSent == CAN_FRAME_SIZE) {
        std::cout << "Vehicle speed data sent successfully." << std::endl;
    } else if (bytesSent == -1) {
        std::cerr << "Error in sending data." << std::endl;
    } else {
        std::cerr << "Unexpected behavior in sending data. Sent bytes: " << bytesSent << std::endl;
    }
    printf("Raw timespec.tv_nsec:%jd.%09ld\n", next_time_.tv_sec, next_time_.tv_nsec);
}

void server::receiveVehicleSpeed() {
    memset(receiveCANFrame, 0, CAN_FRAME_SIZE);

    int bytesReceived = recv(etherNERIClient_socket, receiveCANFrame, CAN_FRAME_SIZE, 0);
    if (bytesReceived == -1) {
        std::cerr << "Failed to receive data" << std::endl;
    } else {
        std::cout << "Received data successfully." << std::endl;
    }
}

void server::messageDecoding() {
    // Implement message decoding logic if necessary
}

void server::epsControlThreadfunc() {
    // Implement EPS control logic if necessary
}

