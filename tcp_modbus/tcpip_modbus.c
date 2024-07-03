#include<stdio.h>
#include<modbus/modbus.h>
#include<error.h>

modbus_t *ctx;
uint8_t board_inputs[24];
int num_outputs = 16;
uint8_t hex_input[num_outputs];
modbus_set_slave(ctx, 1);
    
struct Gpi{
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
}

struct Gpo{
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
}
void send_data(){
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
int set(char input){
	switch (input){
		case input_pin1:
		    hex_input[0]=01;
		    send_data();
		case input_pin2:
		    hex_input[1]=01;
		    send_data();
		case input_pin3:
		
		  
		 
		    
	}
}

int get(){


}

int main{
	
	set(input_pin1);
	
 	return 0;

}
