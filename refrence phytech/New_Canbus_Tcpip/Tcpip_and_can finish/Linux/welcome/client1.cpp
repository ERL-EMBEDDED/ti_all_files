#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <ctime>
#include <iomanip>

const int PORT = 8800;

void printTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
    std::cout << std::put_time(std::localtime(&time), "%T") << ":" << std::setfill('0') << std::setw(3) << milliseconds.count() << ":" << std::setw(9) << nanoseconds.count() << " ";
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Connect to the server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("192.168.3.12"); // Server IP address (change to your server's IP)
    serverAddr.sin_port = htons(PORT);
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Connection failed\n";
        close(clientSocket);
        return 1;
    }

    // Receive numbers from the server
    int receivedNumber;
    while (recv(clientSocket, &receivedNumber, sizeof(receivedNumber), 0) > 0) {
        printTime();
        std::cout << "Received number: " << receivedNumber << std::endl;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}

