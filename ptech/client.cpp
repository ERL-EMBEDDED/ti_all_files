#include "client.h"
#include <iostream>

// Constructor
epsControl::epsControl() {
    clientSocket = -1; // Initialize socket to an invalid value
}

// Destructor
epsControl::~epsControl() {
    if (clientSocket != -1) {
        close(clientSocket);
    }
}

// Method to initialize socket connection
void epsControl::initSocketConnection() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(clientSocket);
        clientSocket = -1;
    }
}

// Method to close socket connection
void epsControl::closeSocketConnection() {
    if (clientSocket != -1) {
        close(clientSocket);
        clientSocket = -1;
    }
}

void epsControl::messageDecoding()
{
    actualVehicleSpeed = 0x0;
    vehicleSpeed = 0x00;
    for(int i=0 ; i < CAN_DATA_SIZE; i++)
    {
	    receiveCANData[i] = receiveCANFrame[i+1];
	    printf("[%c]", receiveCANFrame[i]);
    }
    printf("\n");
    
    vehicleSpeed = vehicleSpeed | ((0xFFFF00 | (receiveCANData[3])) << 24);
    vehicleSpeed = vehicleSpeed | ((0xFFFF00 | (receiveCANData[2])) << 16);
    vehicleSpeed = vehicleSpeed | ((0xFF0000 | (receiveCANData[1])) << 8);
    vehicleSpeed = vehicleSpeed | ((0x000000 | (receiveCANData[0])));

    actualVehicleSpeed = vehicleSpeed;
    printf("actualVehiclespeed [%d]\n", actualVehicleSpeed);
		    
//    memcpy((*(receiveCANData)), (*(receiveCANFrame+1)),CAN_DATA_SIZE);
}


void epsControl::receiveVehicleSpeed() {
    // Receive TCP/IP frame from client
   int bytesReceived = recv(etherNERIClient_socket, receiveCANFrame, CAN_FRAME_SIZE, 0);
    if(bytesReceived > 0) {
        // Data received successfully
        // Decode received frame and extract vehicle speed
        int actualVehicleSpeed;
        memcpy(&actualVehicleSpeed, &receiveCANFrame[1], sizeof(actualVehicleSpeed));
        std::cout << "Received Vehicle Speed: " << actualVehicleSpeed << std::endl;
    } else if (bytesReceived == 0) {
        // No data received
        std::cout << "No data received within "  << std::endl;
    } else {
        // Error occurred during receiving
        std::cerr << "Error in receiving data." << std::endl;
    }
}



int main()
{
	epsControl eps;
	eps.initSocketConnection();
	expectedVehicleSpeed =100;
	while (true)
	{
		//printf("Enter ExpectedSpeed\n");
		//scanf("%d", &expectedVehicleSpeed);
		expectedVehicleSpeed ++;
		if(expectedVehicleSpeed ==500)
		{
			expectedVehicleSpeed=100;
		}

		eps.messageEncoding();
	clock_gettime(CLOCK_MONOTONIC, &next_time_);
    printf("After encoding going to send timespec.tv_nsec:%jd.%09ld\n",next_time_.tv_sec, next_time_.tv_nsec); 
		eps.sendVehicleSpeed();
	clock_gettime(CLOCK_MONOTONIC, &next_time_);
    printf("post reciveing timespec.tv_nsec:%jd.%09ld\n",next_time_.tv_sec, next_time_.tv_nsec); 
		eps.receiveVehicleSpeed();
		//eps.messageDecoding();
        }
	eps.closeSocketConnection();
	return 0;
}

