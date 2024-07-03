// client_main.cpp
#include "client.h"

int main() {
    // Create and connect the client
    ClientTcpip client;
    if (!client.connectToServer()) {
        std::cerr << "Failed to connect client to server\n";
        return 1;
    }

    // Simulate sending and receiving vehicle speed from client to server
    float speedToSend = 60.0; // Example speed value
    client.sendVehicleSpeed(speedToSend);
    client.receiveVehicleSpeed();

    // Disconnect client from server
    client.disconnectFromServer();

    return 0;
}

