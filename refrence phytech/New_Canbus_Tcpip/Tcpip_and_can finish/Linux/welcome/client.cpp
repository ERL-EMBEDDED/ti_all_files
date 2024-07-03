#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

const int IPC_BLOCK_SIZE = 1024;
const int PORT = 8090;

struct Block
{
    int Next;
    int Amount;
    char Data[IPC_BLOCK_SIZE];
};

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Set up server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("192.168.3.12");

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Connection failed\n";
        close(clientSocket);
        return 1;
    }

    // Receive data from server
    Block receivedBlock;
    int bytesReceived = recv(clientSocket, &receivedBlock, sizeof(Block), 0);
    if (bytesReceived == -1) {
        std::cerr << "Error receiving data\n";
        close(clientSocket);
        return 1;
    }

    // Output received data
    std::cout << "Received block with " << receivedBlock.Amount << " bytes of data:\n";
    std::cout.write(receivedBlock.Data, receivedBlock.Amount);
    std::cout << std::endl;

    // Close socket
    close(clientSocket);

    return 0;
}

