#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

const int PORT = 8800;

void clientHandler(int clientSocket) {
    int numToSend = 100;
    while (1) {
        if (send(clientSocket, &numToSend, sizeof(numToSend), 0) == -1) {
            std::cerr << "send() failed\n";
            close(clientSocket);
            return;
        }
        numToSend++;
    }
    close(clientSocket);
}

int main() {
    int listenSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create a socket
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Bind the socket
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);
    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Bind failed\n";
        close(listenSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(listenSocket, 5) == -1) {
        std::cerr << "Listen failed\n";
        close(listenSocket);
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept incoming connections and handle them
    while (true) {
        clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Accept failed\n";
            close(listenSocket);
            return 1;
        }
        std::thread(clientHandler, clientSocket).detach();
    }

    // Close the listening socket
    close(listenSocket);

    return 0;
}

