#ifndef EPSCONTROL_H
#define EPSCONTROL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define SERVER_PORT 9999  // Example server port
#define SERVER_IP "192.168.4.13" // Example server IP
#define CAN_FRAME_SIZE 8

class epsControl {
public:
    epsControl(); // Constructor
    ~epsControl(); // Destructor

    void initSocketConnection(); // Method to initialize socket connection
    void closeSocketConnection(); // Method to close socket connection
    void sendVehicleSpeed(); // Method to send vehicle speed
    void receiveVehicleSpeed(); // Method to receive vehicle speed

private:
    int clientSocket; // Client socket descriptor
    struct sockaddr_in serverAddr; // Server address structure
};

#endif // EPSCONTROL_H

