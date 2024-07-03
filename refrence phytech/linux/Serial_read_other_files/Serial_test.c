#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

int main() {
    const char* canInterface = "/dev/ttyUSB1";
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

    unsigned char send_hex_data[] = {0x00, 0x00, 0x01, 0x23, 0x00,0x02,0x00};
    
    // Write data to the CAN socket
    write(can_fd, send_hex_data, sizeof(send_hex_data));

    return 0;
}

