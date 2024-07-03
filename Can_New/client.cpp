#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int IPC_BLOCK_SIZE = 1024;
const int PORT = 8090;

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

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Connection failed\n";
        close(clientSocket);
        return 1;
    }

    // Prepare data to send
    Block block;
    block.Next = 0;
    block.Amount = 500; // Sending 500 bytes of data
    // Fill Data field with some dummy data
    memset(block.Data, 'A', block.Amount);

    // Send data to the server
    if (send(clientSocket, &block, sizeof(Block), 0) == -1)
    {
        std::cerr << "send() failed\n";
        close(clientSocket);
        return 1;
    }

    // Receive response from the server
    Block response;
    int bytesReceived;
    if ((bytesReceived = recv(clientSocket, &response, sizeof(Block), 0)) == -1)
    {
        std::cerr << "recv() failed\n";
        close(clientSocket);
        return 1;
    }

    std::cout << "Received response from server: " << response.Amount << " bytes\n";

    // Close the socket
    close(clientSocket);

    return 0;
}

