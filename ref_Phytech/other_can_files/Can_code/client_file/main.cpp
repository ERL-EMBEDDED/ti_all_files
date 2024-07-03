#include "client.h"

int main() {
    ClientTcpip tcpip;
    tcpip.connectServer(); 
    tcpip.run();
    tcpip.disconnect(); // Disconnect after exiting the loop
    return 0;
}

