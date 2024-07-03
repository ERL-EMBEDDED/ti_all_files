#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main() {
    const char* canInterface = "/dev/ttyUSB0"; // Replace with the actual device for can0
    int baud_rate = B115200;

    // Open the CAN interface
    int can_fd = open(canInterface, O_RDWR | O_NOCTTY | O_NDELAY);
    if (can_fd == -1) {
        perror("Error opening CAN interface");
        exit(EXIT_FAILURE);
    }

    // Configure CAN interface settings
    struct termios options;
    tcgetattr(can_fd, &options);
    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(can_fd, TCSANOW, &options);

    printf("Reading from %s... Press Ctrl+C to stop.\n", canInterface);

    while (1) {
        unsigned char buffer[256]; // Adjust the buffer size as needed
        ssize_t bytesRead = read(can_fd, buffer, sizeof(buffer));

        if (bytesRead > 0) {
            // Print the received data as hex
            printf("Received (hex): ");
            for (ssize_t i = 0; i < bytesRead; ++i) {
                printf("%02X ", buffer[i]);
            }
            printf("\n");
        }

        // Add a small delay to avoid continuous polling and high CPU usage
        usleep(10000); // 10 milliseconds
    }

    // Close the CAN interface
    close(can_fd);

    return 0;
}

