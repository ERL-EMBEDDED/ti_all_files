// client.h
#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#define SERVER_IP "192.168.4.11"
#define SERVER_PORT 9080

class epsClient {
private:
    int clientSocket;
    struct sockaddr_in serverAddr;

public:
    epsClient();
    ~epsClient();
    void initSocket();
    bool connectToServer();
    int communicateWithServer(std::string& sendMessage);
    void closeConnection();
    int run();
    int SendMessage();
    int enterConfig();
};
    
     
#endif // CLIENT_H

