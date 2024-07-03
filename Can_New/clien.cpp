#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int IPC_BLOCK_SIZE = 1024;
const int PORT = 8888;

struct Block
{
    int Next;
    int Amount;
    char Data[IPC_BLOCK_SIZE];
};

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Set up the server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Connection failed\n";
        close(clientSocket);
        return 1;
    }

    std::cout << "Connected to server\n";

    // Receive the block from the server
    Block block;
    ssize_t bytesReceived;
    if ((bytesReceived = recv(clientSocket, &block, sizeof(Block), 0)) == -1)
    {
        std::cerr << "recv() failed\n";
        close(clientSocket);
        return 1;
    }

    std::cout << "Received block with " << bytesReceived << " bytes of data.\n";

    // Process received data if needed
    // ...

    // Close the socket
    close(clientSocket);

    return 0;
}

