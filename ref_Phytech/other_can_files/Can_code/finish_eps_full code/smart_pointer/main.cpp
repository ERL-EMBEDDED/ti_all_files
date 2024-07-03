// main.cpp
#include "client.h"
#include "can.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <memory>

int main() {
     // Connect to TCP/IP server
    ClientTcpip client;
    client.connectServer();
 
    // Initialize CAN interface
    CanInterface can("can0");
    can.initCan("can0");

    can.sendEncodedData();
    can.decodeAndSend();
    client.receiveVehicleSpeed();
    client.sendVehicleSpeed();

    return 0;
}
