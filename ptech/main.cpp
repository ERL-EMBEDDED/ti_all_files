#include "server.h"
#include <iostream>

int main() {
    // Create an instance of the epsControl class
    client client;

    // Initialize socket connection
    client.initSocketConnection();

    // Send vehicle speed (this is just an example; you can modify it)
    client.sendVehicleSpeed();

    // Receive vehicle speed (this is just an example; you can modify it)
    client.receiveVehicleSpeed();

    // Close socket connection
    client.closeSocketConnection();

    return 0;
}

