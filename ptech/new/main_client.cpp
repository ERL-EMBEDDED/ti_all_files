#include "client.h"

int main() {
    ClientTcpip client;
    if(!client.connectServer()) {
        std::cerr << "Failed to connect to server" << std::endl;
        return 1;
    }

    client.expectedVehicleSpeed = 100; // Initialize expectedVehicleSpeed

    while (true) {
        client.expectedVehicleSpeed++;
        if (client.expectedVehicleSpeed == 500) {
            client.expectedVehicleSpeed = 100;
        }

        client.messageEncoding();
        client.sendVehicleSpeed();
        client.receiveVehicleSpeed();
        client.messageDecoding();

        client.closeSocketConnection(); // Move this inside the loop if needed
    }

    return 0;
}

