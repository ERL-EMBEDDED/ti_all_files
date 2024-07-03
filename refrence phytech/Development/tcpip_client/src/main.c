#include <stdio.h>
#include "tcpip_client.h"
#include <pthread.h>

// Global variable to store the received message
char Tcpip_Receive_Message[1024];

int main() {
    // Thread for TCP/IP communication
    const char *server_ip = "192.168.3.12"; // Update with your server IP
    int server_port = 9999;

    // Call the TCP/IP client function
    runTcpipClient(server_ip, server_port, Tcpip_Receive_Message);

    // Print the received message
    printf("Tcpip Client Receive Message: %s\n", Tcpip_Receive_Message);

    return 0;
}

