#include <stdio.h>
#include <modbus/modbus.h>
#include <stdbool.h>
#include <errno.h>

modbus_t *ctx;
uint8_t board_inputs[24];
int num_outputs = 16;
uint8_t hex_input[16];
int num_inputs = 16;
uint8_t discrete_inputs[16];

struct Gpi {
    bool input_pin[16];
};

struct Gpo {
    bool output_pin[24];
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
    int rc = modbus_write_bits(ctx, 0, num_outputs, hex_input);

    if (rc == -1) {
        fprintf(stderr, "Error writing discrete outputs: %s\n", modbus_strerror(errno));
    } else {
        printf("Hex Input Successfully Written\n");
    }
}

void set_pin(bool *pin_array, int pin_index, bool value, const char *pin_type) {
    if (pin_index >= 0 && pin_index < num_inputs) {
        pin_array[pin_index] = value;
        printf("%s_pin[%d] set to %d\n", pin_type, pin_index, value);
    } else {
        fprintf(stderr, "Invalid %s pin index\n", pin_type);
    }
}

void set_input_pin(struct Gpi *gpi, int pin_index, bool value) {
    set_pin(gpi->input_pin, pin_index, value, "Input");
}

void set_output_pin(struct Gpo *gpo, int pin_index, bool value) {
    set_pin(gpo->output_pin, pin_index, value, "Output");
}

void receive_data() {
    int rc = modbus_read_input_bits(ctx, 0, num_inputs, discrete_inputs);

    if (rc == -1) {
        fprintf(stderr, "Error reading discrete inputs: %s\n", modbus_strerror(errno));
    } else {
        printf("Discrete Inputs: ");
        for (int i = 0; i < num_inputs; i++) {
            printf("%d ", discrete_inputs[i]);
        }
        printf("\n");
    }
}

void get(char input) {
    printf("Pin %c Output is %d\n", input, input_pins.input_pin[input - 'A']);
}

int main() {
    init();

    // Modify input pins
    set_input_pin(&input_pins, 0, true);   // Set the first input pin to true
    set_input_pin(&input_pins, 1, false);  // Set the second input pin to false
    // ... other modifications

    // Modify output pins
    set_output_pin(&output_pins, 0, true);  // Set the first output pin to true
    set_output_pin(&output_pins, 1, false); // Set the second output pin to false
    // ... other modifications

    // Send the updated data after all modifications
    send_data();
    receive_data();
    get('C');

    // Disconnect and free the context
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}

