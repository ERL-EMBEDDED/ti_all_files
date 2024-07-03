//min
#include <stdio.h>
#include <modbus/modbus.h>
#include <errno.h>

int main() {
    modbus_t *ctx;
    uint8_t discrete_inputs[24]; // Assuming you are reading 24 discrete inputs
    uint8_t hex_input[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1};

    // Create a new Modbus context
    ctx = modbus_new_tcp("192.168.250.1", 8080); // Change IP and port as per your Modbus server configuration

    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    // Connect to the Modbus server
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // Set the Modbus slave address
    modbus_set_slave(ctx, 1); // Set the appropriate slave address

    // Write discrete outputs (coils)
    int num_outputs = 16; // Change this based on the number of discrete outputs you want to write
    int rc = modbus_write_bits(ctx, 0, num_outputs, hex_input);

    if (rc == -1) {
        fprintf(stderr, "Error writing discrete outputs: %s\n", modbus_strerror(errno));
    } else {
        printf("Hex Input Successfully Written\n");
    }

    // Disconnect and free the context
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}

