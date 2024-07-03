#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctime>
#include <chrono>
#include <iomanip>

#define PORT 8888
#define BUFFER_SIZE 1024

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_millisec = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    auto now_nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() % 1000000;
    auto tm_info = std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(tm_info, "%T") << ":" << std::setfill('0') << std::setw(3) << now_millisec << ":" << std::setfill('0') << std::setw(9) << now_nanosec;
    return oss.str();
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Forcefully attaching socket to the port 12345
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt failed" << std::endl;
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 12345
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        std::cerr << "Accept failed" << std::endl;
        return -1;
    }

    while (true) {
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0)
            break;

        // Get current time
        std::string current_time = getCurrentTime();
        std::cout << current_time << " Received message: " << buffer << std::endl;
        std::string response = "Hello from server ";

        // Send response
        send(new_socket, response.c_str(), response.length(), 0);

        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
    }

    return 0;
}

