// client.cpp
#include "client.h"
#include <cstring>    // For memset
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <arpa/inet.h> // For socket functions
#include <unistd.h>    // For close()

#include "dbc_parser.h"
#include <vector>

// Constructor
epsClient::epsClient() {
    clientSocket = -1;
    memset(&serverAddr, 0, sizeof(serverAddr));
}

// Destructor
epsClient::~epsClient() {
    if (clientSocket != -1) {
        close(clientSocket);
    }
}

// Initialize socket
void epsClient::initSocket() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
}

// Connect to server
bool epsClient::connectToServer() {
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        return false;
    }
    std::cout << "Connected to server" << std::endl;
    return true;
}

 int epsClient::communicateWithServer(std::string& sendMessage) {    
        // Send JSON string to server
        ssize_t bytesSent = send(clientSocket, sendMessage.c_str(), sendMessage.size(), 0);
        if (bytesSent < 0) {
            std::cerr << "Failed to send message to server" << std::endl;
            return 1;
        }

        // Receive JSON string from server
        const int bufferSize = 1024; // Adjust buffer size as needed
        char buffer[bufferSize];
        ssize_t bytesRead = recv(clientSocket, buffer, bufferSize, 0);
        
        if (bytesRead < 0) {
            std::cerr << "Failed to receive acknowledgment from server" << std::endl;
            return 1;
        } else if (bytesRead == 0) {
            std::cerr << "Server disconnected" << std::endl;
            return 1;
        }
        
        // Process received JSON string (in this case, just print it)
        std::string receivedMessage(buffer, bytesRead);
        std::cout << "Received from server: " << receivedMessage << std::endl;  
        
        return 0;
    }
// Communicate with server       
 int epsClient::SendMessage() {
    float val1, val2, val3;
    
    std::cout << "Enter first float value: ";
    std::cin >> val1;
    
    std::cout << "Enter second float value: ";
    std::cin >> val2;
    
    std::cout << "Enter third float value: ";
    std::cin >> val3;
    
    // Use a stringstream to build the JSON string
    std::ostringstream oss;
    oss << R"({"mode":"run","WheelSpeedFR":)" << val1 << R"(,"WheelSpeedFL":)" << val2 << R"(,"VehicleSpeed":)" << val3 << "}";

    std::string Message = oss.str();
    
    std::cout << "Entered values are: " << val1 << ", " << val2 << ", " << val3 << std::endl;
    std::cout << "Generated JSON message: " << Message << std::endl;
    
    communicateWithServer(Message);
    
    return 0;
}

int epsClient::enterConfig(){
         /*std::string config = R"({
        "config":"yes",
        "data":[
            {
                "name":"VDC_A9:",
                "id_hex":650,
                "length":0,
                "signals":[
                    {
                        "name":"WheelSpeedFR",
                        "start_bit":7,
                        "bit_length":16,
                        "is_big_endian":true,
                        "is_signed":false,
                        "scale":0.041667,
                        "offset":0,
                        "minimum":0,
                        "maximum":2730.58,
                        "unit":"rpm"
                    },
                    {
                        "name":"WheelSpeedFL",
                        "start_bit":23,
                        "bit_length":16,
                        "is_big_endian":true,
                        "is_signed":false,
                        "scale":0.041667,
                        "offset":0,
                        "minimum":0,
                        "maximum":2730.58,
                        "unit":"rpm"
                    },
                    {
                        "name":"VehicleSpeedCRC",
                          "start_bit":63,
                        "bit_length":8,
                        "is_big_endian":true,
                        "is_signed":false,
                        "scale":1,
                        "offset":0,
                        "minimum":0,
                        "maximum":255,
                        "unit":""
                    },
                    {
                        "name":"VehicleSpeedClock",
                        "start_bit":51,
                        "bit_length":4,
                        "is_big_endian":true,
                        "is_signed":false,
                        "scale":1,
                        "offset":0,
                        "minimum":0,
                        "maximum":15,
                        "unit":""
                    },
                    {
                        "name":"VehicleSpeed",
                        "start_bit":39,
                        "bit_length":16,
                        "is_big_endian":true,
                        "is_signed":false,
                        "scale":0.01,
                        "offset":0,
                        "minimum":0,
                        "maximum":655.34,
                        "unit":"km/h"
                    }
                ]
            }
        ]
    })";
    */
     int id =0x28a;
    std::string FILE_NAME="/home/ti/run_yata_client_with_dbc_parser/Batch3_202108.dbc";
    std::string jsonString = confic_yatacan(id,FILE_NAME);
    if (!jsonString.empty()) {
        
        std::cout<< "Dbc file read"<<std::endl;
    
        //std::cout << jsonString << std::endl;
    } else {
        std::cerr << "Message with ID 0x28a not found." << std::endl;
    }
    std::ostringstream oss;
    oss << R"({"mode":"config","data":[)";
    oss<<jsonString;
    oss << "] }";
     std::string config = oss.str();
     
    communicateWithServer(config);
     std::cout << config<< std::endl;
    return 0;

}
int epsClient::run(){
    int choice;
    while (true) {
        std::cout << "Select an option: (1) Enter Config (2) Send Message (3) Exit: ";
        std::cin >> choice;
        
        if (choice == 1) {
            enterConfig();
        } else if (choice == 2) {
            SendMessage();
        } else if (choice == 3) {
            std::cout << "Exiting program." << std::endl;
            break;
        } else {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    }
    return 0;
    }       

// Close connection
void epsClient::closeConnection() {
    if (clientSocket != -1) {
        close(clientSocket);
        clientSocket = -1;
    }
}

