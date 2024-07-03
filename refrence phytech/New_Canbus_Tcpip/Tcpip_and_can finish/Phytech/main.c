#include <stdio.h>
#include "tcpip_client.h"
#include <pthread.h>
#include "can_communication.h"
// Global variable to store the received message
char Tcpip_Receive_Message[1024];

int main() {
    // Thread for TCP/IP communication
    const char *server_ip = "192.168.3.12"; // your server IP
    int server_port = 9999;//your server port

    while(1)
    {
    	// Call the TCP/IP client function
    	runTcpipClient(server_ip, server_port, Tcpip_Receive_Message);

    	// Print the received message
    	printf("Tcpip Client Receive Message: %s\n", Tcpip_Receive_Message);
    
   	 const char *canInterface = "can0"; //can id
    	int bitrate = 250000; //can bus bitrate
    	const char *originalCanMessage = Tcpip_Receive_Message;
    	const char* verificationMessage = "123#000400";
    	const char* expectedVerificationResponse = "can0  00000123   [3]  00 02 00";

    	int result = runCanCommunication(canInterface, bitrate, originalCanMessage,verificationMessage,expectedVerificationResponse);
	}
    return 0;
}

