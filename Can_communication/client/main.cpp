#include "client.h"

int main() {
    ClientTcpip client;

    // Connect to the server
    if (!client.connectServer()) {
        std::cerr << "Failed to connect to server\n";
        return 1;
    }

    std::cout << "Connected to server\n";

    // Send and receive vehicle speed data
    float speedToSend = 60.0;
    client.sendVehicleSpeed(speedToSend);
    float receivedSpeed = client.receiveVehicleSpeed();

    std::cout << "Sent Speed: " << speedToSend << ", Received Speed: " << receivedSpeed << std::endl;

    // Disconnect from the server
    client.disconnectServer();

    std::cout << "Disconnected from server\n";

    return 0;
}

