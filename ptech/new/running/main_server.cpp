#include "server.h"

int main() {
    server myServer;

    // Initialize the socket connection
    myServer.initSocketConnection();

    // Example logic for sending and receiving vehicle speed
    for (expectedVehicleSpeed = 100; expectedVehicleSpeed < 500; ++expectedVehicleSpeed) {
        myServer.messageEncoding();
        myServer.sendVehicleSpeed();
        myServer.receiveVehicleSpeed();
    }

    // Close the socket connection
    myServer.closeSocketConnection();

    return 0;
}

