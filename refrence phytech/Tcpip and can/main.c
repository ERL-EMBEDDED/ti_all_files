// main.c
#include "can_communication.h"
#include "tcpip_client.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    
    const char* serverIp = "192.168.3.12";
    int serverPort = 9999;
    char tcpIpReceiveMessage[1024];  // Adjust the size as needed
    
        // Run TCP/IP client
    runTcpipClient(serverIp, serverPort, tcpIpReceiveMessage);
    
    // Example values, replace with actual values or user input
    const char* canInterface = "can0";
    int bitrate = 500000;
    const char* originalCanMessage = tcpIpReceiveMessage;

    // Run CAN communication
    if (runCanCommunication(canInterface, bitrate, originalCanMessage) != 0) {
        fprintf(stderr, "Error in CAN communication\n");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}

