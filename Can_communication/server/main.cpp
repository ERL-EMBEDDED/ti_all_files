#include "server.h"

int main() {
    ServerTcpip server;

    // Start the server
    if (!server.startServer()) {
        std::cerr << "Failed to start server\n";
        return 1;
    }

    std::cout << "Server started. Waiting for connections...\n";

    // Run the server
    server.run();

    // Stop the server
    server.stopServer();

    std::cout << "Server stopped\n";

    return 0;
}

