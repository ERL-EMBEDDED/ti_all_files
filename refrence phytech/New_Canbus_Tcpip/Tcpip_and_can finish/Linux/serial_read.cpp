#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <unistd.h> // For sleep function
#include <fcntl.h> // For file control options
#include <termios.h> // For terminal I/O interface
#include <chrono>

void printTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::chrono::duration<double> seconds = now.time_since_epoch();
    std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(seconds);
    std::chrono::nanoseconds nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(seconds) % std::chrono::seconds(1);
    std::cout << std::put_time(std::localtime(&time), "%F %T") << "." << std::setfill('0') << std::setw(3) << milliseconds.count() % 1000 << " " << std::setw(9) << nanoseconds.count() << ": ";
}

// Helper function to print hexadecimal data
void printHex(const unsigned char* data, size_t length) {
    std::cout << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        std::cout << std::setw(2) << static_cast<unsigned>(data[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}

int main() {
    const char *device = "/dev/ttyUSB4"; // Serial port device name

    // Open the serial port
    int serialPort = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialPort == -1) {
        std::cerr << "Error: Unable to open serial port." << std::endl;
        return 1;
    }

    // Configure the serial port settings
    struct termios tty;
    tcgetattr(serialPort, &tty);
    cfsetospeed(&tty, B115200); // Set baud rate (115200 baud)
    cfsetispeed(&tty, B115200);
    tty.c_cflag &= ~PARENB; // No parity
    tty.c_cflag &= ~CSTOPB; // One stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS; // No hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable receiver, ignore modem control lines
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control
    tty.c_lflag = 0; // No signaling characters, no echo, no canonical processing
    tty.c_oflag = 0; // No output processing
    tty.c_cc[VMIN] = 1; // Read blocks until at least 1 char received
    tty.c_cc[VTIME] = 0; // Timeout in 0 deciseconds

    tcsetattr(serialPort, TCSANOW, &tty); // Apply settings

    // Read data from the serial port
    unsigned char buffer[16]; // Assuming max size of CAN frame is 16 bytes
    ssize_t bytesRead;

    while (true) {
        bytesRead = read(serialPort, buffer, sizeof(buffer));
        if (bytesRead > 0) {
            // Data read successfully
            printTime();
            std::cout << "Received data: ";
            printHex(buffer, bytesRead); // Printing all received bytes
        } else if (bytesRead < 0) {
            // Error while reading
            std::cerr << "Error reading from serial port." << std::endl;
            break; // Exit loop on error
        } 
    }

    // Close the serial port
    close(serialPort);

    return 0;
}

