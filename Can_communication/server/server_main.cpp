// server_main.cpp
#include "server.h"

int main() {
    // Create and start the server
    ServerTcpip server;
    if (!server.startServer()) {
        std::cerr << "Failed to start server\n";
        return 1;
    }

    // Run the server
    server.run();

    // Stop the server
    server.stopServer();

    return 0;
}

