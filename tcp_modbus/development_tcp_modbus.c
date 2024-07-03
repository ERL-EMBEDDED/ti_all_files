//m_t_i_o   verify
#include <stdio.h>
#include <modbus/modbus.h>
#include <stdbool.h>
#include <errno.h>

modbus_t *ctx;
uint8_t board_inputs[16];
int num_outputs = 24;
uint8_t hex_input[16];
int num_inputs = 16;
uint8_t discrete_output[24];

struct Gpi {
    bool input_pin1;
    bool input_pin2;
    bool input_pin3;
    bool input_pin4;
    bool input_pin5;
    bool input_pin6;
    bool input_pin7;
    bool input_pin8;
    bool input_pin9;
    bool input_pin10;
    bool input_pin11;
    bool input_pin12;
    bool input_pin13;
    bool input_pin14;
    bool input_pin15;
    bool input_pin16;
};

struct Gpo {
    bool output_pin1;
    bool output_pin2;
    bool output_pin3;
    bool output_pin4;
    bool output_pin5;
    bool output_pin6;
    bool output_pin7;
    bool output_pin8;
    bool output_pin9;
    bool output_pin10;
    bool output_pin11;
    bool output_pin12;
    bool output_pin13;
    bool output_pin14;
    bool output_pin15;
    bool output_pin16;
    bool output_pin17;
    bool output_pin18;
    bool output_pin19;
    bool output_pin20;
    bool output_pin21;
    bool output_pin22;
    bool output_pin23;
    bool output_pin24;
};

struct Gpi input_pins = {.input_pin = {false, false, false, /* ... */}};
struct Gpo output_pins = {.output_pin = {false, false, false, /* ... */}};

int init() {
    // Create a new Modbus context
    ctx = modbus_new_tcp("192.168.250.1", 8080);

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
    modbus_set_slave(ctx, 1);

    // Initialize hex_input to zeros
    for (int i = 0; i < num_outputs; i++) {
        hex_input[i] = 0x00;
    }

    return 0;
}

void send_data() {
    int rc = modbus_write_bits(ctx, 0, num_inputs, hex_input);

    if (rc == -1) {
        fprintf(stderr, "Error writing discrete outputs: %s\n", modbus_strerror(errno));
    } else {
        printf("Hex Input Successfully Written\n");
    }
}

void set_pin(int pin_index, bool value, const char *pin_type) {
    if (pin_index >= 0 && pin_index < num_inputs) {
        hex_input[pin_index] = value;
        printf("%s_pin[%d] set to %d\n", pin_type, pin_index, value);
    } else {
        fprintf(stderr, "Invalid %s pin index\n", pin_type);
    }
}

void set_input_pin(struct Gpi *gpi, int pin_index, bool value) {
    set_pin( pin_index, value, "Output");
}

void receive_data() {
    int rc = modbus_read_input_bits(ctx, 0, num_outputs, discrete_output);

    if (rc == -1) {
        fprintf(stderr, "Error reading discrete inputs: %s\n", modbus_strerror(errno));
    } else {
        printf("Discrete Inputs: ");
        for (int i = 0; i < num_outputs; i++) {
            printf("%d ", discrete_output[i]);
        }
        printf("\n");
    }
}

void get(struct Gpo *gpo, int pin_index, int *data) {
    receive_data();
    *data=discrete_output[pin_index];
    printf("Pin %d Output is %d\n", pin_index, discrete_output[pin_index]);
    
}

int main() {
    init();

    // Modify output pins
    set_input_pin(&input_pins, 0, true);  // Set the first output pin to true
    set_input_pin(&input_pins, 1, true); // Set the second output pin to false
    // ... other modifications

    // Send the updated data after all modifications
    send_data();
    receive_data();

    int outputData;
    get(&output_pins, 3, &outputData);
    printf("input data %d\n", outputData);

    // Disconnect and free the context
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}

