#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "tcpip_client.h"
#include <pthread.h>

// Global variable to store the received message
char Tcpip_Receive_Message[1024];

void printCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm *timeinfo;
    timeinfo = localtime(&tv.tv_sec);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "[%H:%M:%S", timeinfo);

    printf("%s:%03ld] ", buffer, tv.tv_usec / 1000);
}

int main() {
    // Thread for TCP/IP communication
    const char *server_ip = "192.168.3.12"; // Update with your server IP
    int server_port = 9999;
    pthread_t tcpip_thread;

    while (1) {
        // Create a structure to hold the thread arguments
        struct ThreadArgs threadArgs;
        threadArgs.server_ip = server_ip;
        threadArgs.server_port = server_port;
        threadArgs.Tcpip_Receive_Message = Tcpip_Receive_Message;

        // Pass the structure as an argument to the thread
        pthread_create(&tcpip_thread, NULL, Tcpip_client_thread, (void *)&threadArgs);

        // Main thread continues with other tasks or waits for completion

        // Join the TCP/IP thread
        pthread_join(tcpip_thread, NULL);

        // Print the current time and received message
        printCurrentTime();
        printf("Tcpip_Receive_Message is : %s\n", Tcpip_Receive_Message);
    }

    return 0;
}

