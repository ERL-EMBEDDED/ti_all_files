#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

int main() {
    const char* canInterface = "/dev/ttyUSB0";
    int baud_rate = B115200;

    int can_fd = open(canInterface, O_RDWR | O_NOCTTY | O_NDELAY);
    if (can_fd == -1) {
        perror("Error opening CAN interface");
        exit(EXIT_FAILURE);
    }

    struct termios options;
    tcgetattr(can_fd, &options);
    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(can_fd, TCSANOW, &options);

    printf("Reading from %s... Press Ctrl+C to stop.\n", canInterface);

    while (1) {
        unsigned char buffer[256];
        ssize_t bytesRead = read(can_fd, buffer, sizeof(buffer));

        if (bytesRead > 0) {
            // Process the raw binary data directly
            printf("Received (hex): ");
            for (ssize_t i = 0; i < bytesRead; ++i) {
                printf("%02X", buffer[i]);
            }
            printf("\n");
            printf("receive message finish\n");
	     
            // Interpret the raw binary data as needed

            // Check if the received data matches a specific condition
            // For example, if the received data is "00000123000400", send "123#000200"
     
                unsigned char send_hex_data[] = {0x00, 0x00, 0x01, 0x23, 0x00,0x02,0x00};
                write(can_fd, send_hex_data, sizeof(send_hex_data));
                printf("Can Message Send \n");
        }
    }

    close(can_fd);

    return 0;
}

