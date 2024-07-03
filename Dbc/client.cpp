#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 1144
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "192.168.3.12", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    int number = 100;
    while (true) {
        // Convert number to string and send message
        std::string message = std::to_string(number);
        send(sock, message.c_str(), message.length(), 0);
        std::cout << "Message sent to server: " << message << std::endl;

        // Receive response
        valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            std::cerr << "Server disconnected" << std::endl;
            break;
        }
        std::cout << "Server response: " << buffer << std::endl;

        // Increment number for the next message
        number++;

        // Clear the buffer for the next message
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(sock);

    return 0;
}

