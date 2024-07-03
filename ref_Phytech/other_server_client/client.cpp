#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

const char* server_ip = "192.168.3.12"; // Change this to the actual server IP
const int server_port = 9999; // Change this to match the server port
const int CAN_FRAME_SIZE = 5;

void sendVehicleSpeed(int socket, int speed) {
    unsigned char sendCANFrame[CAN_FRAME_SIZE];
    memset(sendCANFrame, 0, CAN_FRAME_SIZE);
    sendCANFrame[0] = 0x01; // Example CAN message ID
    memcpy(&sendCANFrame[1], &speed, sizeof(speed));

    int bytesSent = send(socket, sendCANFrame, CAN_FRAME_SIZE, 0);
    if (bytesSent == CAN_FRAME_SIZE) {
        std::cout << "Vehicle speed data sent successfully." << std::endl;
    } else if (bytesSent == -1) {
        std::cerr << "Error in sending data." << std::endl;
    } else {
        std::cerr << "Unexpected behavior in sending data. Sent bytes: " << bytesSent << std::endl;
    }
}

void receiveVehicleSpeed(int socket) {
    unsigned char receiveCANFrame[CAN_FRAME_SIZE];
    int bytesReceived = recv(socket, receiveCANFrame, CAN_FRAME_SIZE, 0);
    if (bytesReceived > 0) {
        int receivedSpeed;
        memcpy(&receivedSpeed, &receiveCANFrame[1], sizeof(receivedSpeed));
        std::cout << "Received Vehicle Speed: " << receivedSpeed << std::endl;
    } else if (bytesReceived == 0) {
        std::cout << "No data received." << std::endl;
    } else {
        std::cerr << "Error in receiving data." << std::endl;
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connection to server failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server." << std::endl;

    // Example usage
    int expectedSpeed = 100;
    while (true) {
        sendVehicleSpeed(client_socket, expectedSpeed);
        receiveVehicleSpeed(client_socket);
        
        expectedSpeed++;
        if (expectedSpeed == 500) {
            expectedSpeed = 100;
        }
        
        sleep(1); // Simulate some delay
    }

    // Close the socket
    close(client_socket);
    return 0;
}

