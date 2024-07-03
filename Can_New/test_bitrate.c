#include <stdio.h>
#include <stdint.h>

// Function to send a 16-bit integer value within the range of 0 to 500
void sendValue(uint16_t value) {
    // Scale the value to fit within 16 bits (0 to 65535)
    uint16_t scaledValue = (uint16_t)((value / 500.0) * 65535);

    // Here you can send the scaledValue using your communication method
    printf("Sending scaled value: %hu\n", scaledValue);

    // Example: sending data over a socket
    // send(socket_fd, &scaledValue, sizeof(scaledValue), 0);
}

int main() {
    // Example value in the range [0, 500]
    uint16_t value = 500;

    // Send the value
    sendValue(value);

    return 0;
}

