#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono> // For time calculations
#include <iomanip> // For formatting output
#include <thread>

const int IPC_BLOCK_SIZE = 1024;
const int PORT = 8090;

struct Block
{
    int Next;
    int Amount;
    char Data[IPC_BLOCK_SIZE];
};

void printCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::cout << std::put_time(std::localtime(&time), "%T") << ":" << std::setfill('0') << std::setw(3) << milliseconds.count() << " ";
}

void clientHandler(int clientSocket)
{
    Block block;
    int bytesReceived;

    // Receive the block from the client
    if ((bytesReceived = recv(clientSocket, &block, sizeof(Block), 0)) == -1)
    {
        std::cerr << "recv() failed\n";
        close(clientSocket);
        return;
    }

    // Simulating processing of client data
    printCurrentTime();
    std::cout << "Received block with " << block.Amount << " bytes of data.\n";
    // Process data...

    // Send response back to the client
    if (send(clientSocket, &block, sizeof(Block), 0) == -1)
    {
        std::cerr << "send() failed\n";
    }

    close(clientSocket);
}

int main()
{
    int listenSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create a socket
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Bind the socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Bind failed\n";
        close(listenSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(listenSocket, 5) == -1)
    {
        std::cerr << "Listen failed\n";
        close(listenSocket);
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept incoming connections and handle them
    while (true)
    {
        clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1)
        {
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

