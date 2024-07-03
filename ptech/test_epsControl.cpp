#include "epsControl.h"
#include <iostream>

int main() {
    // Create an instance of the epsControl class
    epsControl control;

    // Initialize socket connection
    control.initSocketConnection();

    // Send vehicle speed (this is just an example; you can modify it)
    control.sendVehicleSpeed();

    // Receive vehicle speed (this is just an example; you can modify it)
    control.receiveVehicleSpeed();

    // Close socket connection
    control.closeSocketConnection();

    return 0;
}

