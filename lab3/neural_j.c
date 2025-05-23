/*
Names: Joshua Livergood and Danny Moreno
Lab3 
Back Propogation Neural Network for part 2 
*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "pid.h"

#define MAX_NODES 3
#define OUT_NODES 2
#define HIDDEN_NODES 3
#define NUM_INPUTS 2

#define MAX_SAMPLES 250
u08 sensor_readings[MAX_SAMPLES][2]; // left and right values
int sample_count = 0;

typedef struct {
    int numInput;
    float inputs[MAX_NODES];
    float weights[MAX_NODES];
    float bias;
    float output;
} Node;

Node hidden_nodes[HIDDEN_NODES];
Node output_nodes[OUT_NODES];
Node hidden_new[HIDDEN_NODES];
Node output_new[OUT_NODES];
struct motor_command compute_proportional(uint8_t left, uint8_t right, uint8_t left_old, uint8_t right_old);
struct motor_command compute_neural_network(uint8_t left, uint8_t right);
void train_neural_network(uint8_t numEpochs);
void initialize_network(Node *outNodes, Node *hiddenNodes);
void set_motors(u08 l_speed, u08 r_speed);

void set_motors(u08 l_speed, u08 r_speed){
    motor(0, l_speed);
    motor(1, r_speed);
}


void controller(){
    //start in proportional
    int mode = 0; //0 for prop control, 1 for data cap mode, 2 for training, 3 for run neural network
    uint16_t num_epochs;
    
    u08 left;
    u08 right;
    u08 left_old = 0;
    u08 right_old = 0;

    initialize_network(output_nodes, hidden_nodes);

    while (mode == 0){
        //proportional control
        lcd_cursor(0,0);
        print_string("proport");

        left = getLeft();
        right = getRight();

        struct motor_command expected = compute_proportional(left, right, left_old, right_old);
        set_motors(expected.l_speed, expected.r_speed);

        left_old = left;
        right_old = right;

        if (get_btn() == 1 && mode == 0) {
            mode = 1; // Switch to data capture
        }
        _delay_ms(100);  // slight delay before next sample

    }
    clear_screen();
    while (1){
        if (mode == 1){
            clear_screen();
            lcd_cursor(0, 0);
            print_string("Data Cap");
            sample_count = 0;
            set_motors(0,0);

            // uint8_t left_old = getLeft();
            // uint8_t right_old = getRight();
            _delay_ms(100);  // slight delay before next sample

            while (get_btn() != 1) {
                if (sample_count < MAX_SAMPLES) {
                    uint8_t left = getLeft();
                    uint8_t right = getRight();

                    sensor_readings[sample_count][0] = left;
                    sensor_readings[sample_count][1] = right;
                    //sensor_readings[sample_count][2] = left_old;
                    //sensor_readings[sample_count][3] = right_old;

                    // Update previous values
                    // left_old = left;
                    // right_old = right;

                    lcd_cursor(0, 1);
                    print_num(sample_count);
                    sample_count++;

                    _delay_ms(50);
                } else {
                    lcd_cursor(0, 1);
                    print_string("complet");
                }
                  // avoid over-sampling
            }
            _delay_ms(100);
            mode = 2;
        }
        else if (mode == 2){
            clear_screen();
            lcd_cursor(0, 0);
            print_string("Training");

            set_motors(0,0);

             // or get this from accelerometer
            mode = 3;
            //get num epochs with accelerometer
            num_epochs = 0;
            uint8_t x;
            u08 btn_val = 0;
            u16 i;

            _delay_ms(100);

            while(btn_val != 1){
                x = get_accel_x();

                if ((x <= 254 && x >= 250) || (x <= 5)){
                    //Flat
                } else if (x < 252 && x > 185) {
                    if (num_epochs >= 10) num_epochs -= 10; //Tilted towards 
                } else if (x <= 60) {
                    num_epochs += 10; //Tilted away
                }
                clear_screen();
                lcd_cursor(0, 0);
                print_string("Training");
                lcd_cursor(0, 1);
                print_num(num_epochs);
                for(i = 0; i < 50; i++){
                    _delay_ms(10);
                    btn_val = get_btn();
                    if(btn_val == 1){
                        break;
                    }
                }
                
            }

            clear_screen();
            lcd_cursor(0, 0);
            print_string("in proc");
            _delay_ms(100);

            train_neural_network(num_epochs);//call training function with sensor_readings as argument

        }
        else if (mode == 3){
            clear_screen();
            lcd_cursor(0, 0);
            print_string("NN Follow");
            _delay_ms(50);

            while (get_btn() != 1) {
                uint8_t left = getLeft();
                uint8_t right = getRight();
                struct motor_command cmd = compute_neural_network(left, right);
                display_motor(cmd, left, right);
                set_motors(cmd.l_speed, cmd.r_speed);
                _delay_ms(30);
            }
            mode = 2; // go back to training mode
            _delay_ms(100);

        }
        else{
            //default
        }
    }
}

void initialize_network(Node *outNodes, Node *hiddenNodes){

    u08 nodeI;
    u08 weightI;

    for(nodeI = 0; nodeI < HIDDEN_NODES; nodeI++){
        for(weightI = 0; weightI < NUM_INPUTS; weightI++){
            hiddenNodes[nodeI].weights[weightI] = ((float) rand()) / ((float) RAND_MAX);
        }
        hiddenNodes[nodeI].bias = ((float) rand()) / ((float) RAND_MAX);
    }
    
    for(nodeI = 0; nodeI < OUT_NODES; nodeI++){
        for(weightI = 0; weightI < HIDDEN_NODES; weightI++){
            outNodes[nodeI].weights[weightI] = ((float) rand()) / ((float) RAND_MAX);
        }
        outNodes[nodeI].bias = ((float) rand()) / ((float) RAND_MAX);

    }

    
}


struct motor_command compute_neural_network(uint8_t left, uint8_t right) {
    struct motor_command result;

    // Normalize sensor inputs
    float inputs[NUM_INPUTS];
    inputs[0] = left / 255.0;
    inputs[1] = right / 255.0;

    // printf("\ninput values-> l: %f; r: %f\n", inputs[0], inputs[1]);

    // Forward pass: Hidden layer
    for (int i = 0; i < HIDDEN_NODES; i++) {
        float sum = 0;
        for (int j = 0; j < NUM_INPUTS; j++) {
            sum += inputs[j] * hidden_nodes[i].weights[j];
        }
        sum += hidden_nodes[i].bias;
        hidden_nodes[i].output = 1.0 / (1.0 + exp(-sum));
    }

    // Forward pass: Output layer
    for (int i = 0; i < OUT_NODES; i++) {
        float sum = 0;
        for (int j = 0; j < HIDDEN_NODES; j++) {
            sum += hidden_nodes[j].output * output_nodes[i].weights[j];
        }
        sum += output_nodes[i].bias;
        output_nodes[i].output = 1.0 / (1.0 + exp(-sum));
    }

    // Scale to motor speed range (e.g. 0-255)
    result.l_speed = (uint8_t)(output_nodes[0].output * 100);
    result.r_speed = (uint8_t)(output_nodes[1].output * 100);


    return result;
}

float calculate_out_value(float target, float out, float input){
    float dout;
    float dnet; // represents derivative of sigmoid function
    float dweight; // derivative of weight

    dout = out - target;
    dnet = out * (1 - out);
    dweight = input; 

    return dout * dnet * dweight;
}

float calculate_hidden_value(float *targets, float out, float input, Node *output_nodes, u08 weightI){
    float dout = 0;
    float doutput_e; // references values calculated for output nodes
    float doutput_net; // references values calculated for output nodes
    float dnet;
    float dweight;
    // float target;

    u08 outI;
    
    // caluclates error for output nodes
    for(outI = 0; outI < OUT_NODES; outI++){
        doutput_net = output_nodes[outI].output * (1 - output_nodes[outI].output);
        doutput_e = output_nodes[outI].output - targets[outI];

        dout += doutput_net * doutput_e * output_nodes[outI].weights[weightI];
    }
    
    dnet = out * (1 - out);
    dweight = input; 

    return dout * dnet * dweight;
}

// }
void train_neural_network(uint8_t numEpochs) {
    float alpha = 0.3;
    const float alpha_change = (alpha-0.01) / numEpochs;
    float target;
    u08 nodeI;
    u08 h;

    for (int epoch = 0; epoch < numEpochs; epoch++) {
        for (int i = 0; i < MAX_SAMPLES; i++) {
            // Normalize inputs
            float inputs[NUM_INPUTS];
            float targets[OUT_NODES];
            inputs[0] = ((float)sensor_readings[i][0]) / 255.0; // left 
            inputs[1] = ((float)sensor_readings[i][1]) / 255.0; // right

            float target_left, target_right;
            struct motor_command expected;
            // PID expected values
            if(i > 0){
                expected = compute_proportional(sensor_readings[i][0], sensor_readings[i][1], sensor_readings[i-1][0], sensor_readings[i-1][1]);

            } else {
                expected = compute_proportional(sensor_readings[i][0], sensor_readings[i][1], sensor_readings[0][0], sensor_readings[0][1]);

            }

            // _delay_ms(10);

            target_left = expected.l_speed / 100.0;
            target_right = expected.r_speed / 100.0;
            targets[0] = target_left;
            targets[1] = target_right;

            // update output values
            // Update output values for hidden layer
            for (int h = 0; h < HIDDEN_NODES; h++) {
                float sum = 0;
                for (int j = 0; j < NUM_INPUTS; j++) {
                    sum += inputs[j] * hidden_nodes[h].weights[j];
                }
                sum += hidden_nodes[h].bias;
                hidden_nodes[h].output = 1.0 / (1.0 + exp(-sum));
            }

            // update output values for output layer
            for (int o = 0; o < OUT_NODES; o++) {
                float sum = 0;
                for (int j = 0; j < HIDDEN_NODES; j++) {
                    sum += hidden_nodes[j].output * output_nodes[o].weights[j];
                }
                sum += output_nodes[o].bias;
                output_nodes[o].output = 1.0 / (1.0 + exp(-sum));
            }

            // printf("Left Expected: %f; Left Compute: %f\nRight Expected: %f; Right Compute: %f\n\n", targets[0], output_nodes[0].output, targets[1] , output_nodes[1].output);


            // back propogation first
            for(nodeI = 0; nodeI < OUT_NODES; nodeI++){
                // target = (nodeI == 0) ? target_left : target_right;
                // iterate through each value in output node weights[]

                //for each hidden node, there is a related input and weight
                for(h = 0; h < HIDDEN_NODES; h++){
                    output_new[nodeI].weights[h] = calculate_out_value(targets[nodeI], output_nodes[nodeI].output, hidden_nodes[h].output);
                }
                // calculate bias 
                output_new[nodeI].bias = calculate_out_value(targets[nodeI], output_nodes[nodeI].output, 1.0);
            }

            // hidden node layer
            for(nodeI = 0; nodeI < HIDDEN_NODES; nodeI++){
                // h1 takes in input from input 1
                for(h = 0; h < NUM_INPUTS; h++){
                    hidden_new[nodeI].weights[h] = calculate_hidden_value(targets, hidden_nodes[nodeI].output, inputs[h], output_nodes, nodeI);
                }

                // calculates new bias
                hidden_new[nodeI].bias = calculate_hidden_value(targets, hidden_nodes[nodeI].output, 1.0, output_nodes, nodeI);

            }

            // UPDATE NODES
            // Update weights and biases
            for (int o = 0; o < OUT_NODES; o++) {
                for (int h = 0; h < HIDDEN_NODES; h++) {
                    output_nodes[o].weights[h] -= alpha * output_new[o].weights[h];
                }
                output_nodes[o].bias -= alpha * output_new[o].bias;
            }

            for (int h = 0; h < HIDDEN_NODES; h++) {
                for (int j = 0; j < NUM_INPUTS; j++) {
                    hidden_nodes[h].weights[j] -= alpha * hidden_new[h].weights[j];
                }
                hidden_nodes[h].bias -= alpha * hidden_new[h].bias;
            }

            float error = 0;
            for (int k = 0; k < OUT_NODES; k++) {
                float e = output_nodes[k].output - targets[k];
                error += e * e;
            }
            printf("Epoch %d, Sample %d, Error: %.4f\n", epoch, i, error);

        }
        alpha -= alpha_change;
    }
}


int main(void) {
    init();        
    init_adc();    
    while(1) {
        controller();
    }
    return 0;
}
