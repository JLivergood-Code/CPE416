/*
Names: Joshua Livergood and Danny Moreno
Lab3, part 2 
Script used to test and output neural network values
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#define MAX_NODES 3
#define OUT_NODES 2
#define HIDDEN_NODES 3
#define NUM_INPUTS 2

#define CALIBRATION_L 15
#define CALIBRATION_R 15

typedef unsigned char u08;
typedef unsigned int u16;

#define MAX_SAMPLES 10
u08 sensor_readings[MAX_SAMPLES][2]; // left and right values
int sample_count = 0;

typedef struct {
    int numInput;
    float inputs[MAX_NODES];
    float weights[MAX_NODES];
    float bias;
    float output;
} Node;

struct motor_command{

    uint8_t l_speed;
    uint8_t r_speed;
};

Node hidden_nodes[HIDDEN_NODES];
Node output_nodes[OUT_NODES];
Node hidden_new[HIDDEN_NODES];
Node output_new[OUT_NODES];
struct motor_command compute_proportional(uint8_t left, uint8_t right, uint8_t left_old, uint8_t right_old);
struct motor_command compute_neural_network(uint8_t left, uint8_t right);
void train_neural_network(uint8_t numEpochs);
void initialize_network(Node *outNodes, Node *hiddenNodes);

struct motor_command compute_proportional(uint8_t left, uint8_t right, u08 left_old, u08 right_old){
    struct motor_command motor_val;
    int lError, rError, lErrorOld, rErrorOld;
    //int error;

    float kP = 0.055;
    float kD = 0.008; 
    // float kI = 0;
    
    float pLeft, dLeft; //, iLeft;
    float pRight, dRight; //, iRight;

    lError = left - right;
    lErrorOld = left_old - right_old; 
        
    pLeft = kP * lError;
    dLeft = kD * (lError - lErrorOld);
    //lErrorOld = lError;

    // ======================================================================//
    // RIGHT
    // THIS all technically is just a copy of the left motor

    rError = right - left; 
    rErrorOld = right_old - left_old; 

    pRight = kP * rError;
    dRight = kD * (rError - rErrorOld);
    // rErrorOld = rError;

    motor_val.l_speed = pLeft + dLeft + CALIBRATION_L;
    motor_val.r_speed = pRight + dRight + CALIBRATION_R;

    return motor_val;
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
    struct motor_command compute;
    struct motor_command expected;

    sensor_readings[0][0] = 15;
    sensor_readings[0][1] = 15;

    sensor_readings[1][0] = 30;
    sensor_readings[1][1] = 30;

    sensor_readings[2][0] = 15;
    sensor_readings[2][1] = 180;

    sensor_readings[3][0] = 15;
    sensor_readings[3][1] = 180;

    sensor_readings[4][0] = 15;
    sensor_readings[4][1] = 180;

    sensor_readings[5][0] = 15;
    sensor_readings[5][1] = 180;

    sensor_readings[6][0] = 180;
    sensor_readings[6][1] = 15;

    sensor_readings[7][0] = 180;
    sensor_readings[7][1] = 15;

    sensor_readings[8][0] = 180;
    sensor_readings[8][1] = 15;

    sensor_readings[9][0] = 180;
    sensor_readings[9][1] = 15;

    srand(time(NULL));

    initialize_network(output_nodes, hidden_nodes);

    for(int nodeI = 0; nodeI < HIDDEN_NODES; nodeI++){
        printf("h i: %d, weights[0]: %f, weights[1]: %f, bias: %f\n", nodeI, hidden_nodes[nodeI].weights[0],hidden_nodes[nodeI].weights[1], hidden_nodes[nodeI].bias );
    }
    for(int nodeI = 0; nodeI < OUT_NODES; nodeI++){
        printf("o i: %d, weights[0]: %f, weights[1]: %f, weights[2]: %f, bias: %f\n", nodeI, output_nodes[nodeI].weights[0], output_nodes[nodeI].weights[1], output_nodes[nodeI].weights[2], output_nodes[nodeI].bias );
    }

    train_neural_network(50);
    printf("training pt1 \n\n");


    // for(int nodeI = 0; nodeI < HIDDEN_NODES; nodeI++){
    //     printf("h i: %d, weights[0]: %f, weights[1]: %f, bias: %f\n", nodeI, hidden_nodes[nodeI].weights[0],hidden_nodes[nodeI].weights[1], hidden_nodes[nodeI].bias );
    // }
    // for(int nodeI = 0; nodeI < OUT_NODES; nodeI++){
    //     printf("o i: %d, weights[0]: %f, weights[1]: %f, weights[2]: %f, bias: %f\n", nodeI, output_nodes[nodeI].weights[0], output_nodes[nodeI].weights[1], output_nodes[nodeI].weights[2], output_nodes[nodeI].bias );
    // }

    printf("\nValues\n");
    for(int i = 0; i < 10; i++){
        compute = compute_neural_network(sensor_readings[i][0], sensor_readings[i][1]);
        if(i > 0){
            expected = compute_proportional(sensor_readings[i][0], sensor_readings[i][1], sensor_readings[i-1][0], sensor_readings[i-1][1]);

        } else {
            expected = compute_proportional(sensor_readings[i][0], sensor_readings[i][1], sensor_readings[0][0], sensor_readings[0][1]);

        }

        // printf("Left Expected: %d; Left Compute: %d\nRight Expected: %d; Right Compute: %d\n\n", expected.l_speed, compute.l_speed, expected.r_speed, compute.r_speed);
    }

    printf("training pt2 \n\n");

    train_neural_network(100);
    
    for(int nodeI = 0; nodeI < HIDDEN_NODES; nodeI++){
        printf("h i: %d, weights[0]: %f, weights[1]: %f, bias: %f\n", nodeI, hidden_nodes[nodeI].weights[0],hidden_nodes[nodeI].weights[1], hidden_nodes[nodeI].bias );
    }
    for(int nodeI = 0; nodeI < OUT_NODES; nodeI++){
        printf("o i: %d, weights[0]: %f, weights[1]: %f, weights[2]: %f, bias: %f\n", nodeI, output_nodes[nodeI].weights[0], output_nodes[nodeI].weights[1], output_nodes[nodeI].weights[2], output_nodes[nodeI].bias );
    }

    printf("\nValues\n");


    for(int i = 0; i < 10; i++){
        compute = compute_neural_network(sensor_readings[i][0], sensor_readings[i][1]);
        if(i > 0){
            expected = compute_proportional(sensor_readings[i][0], sensor_readings[i][1], sensor_readings[i-1][0], sensor_readings[i-1][1]);

        } else {
            expected = compute_proportional(sensor_readings[i][0], sensor_readings[i][1], sensor_readings[0][0], sensor_readings[0][1]);

        }
        printf("Sensor Readings-> l: %d; r: %d\n", sensor_readings[i][0], sensor_readings[i][1]);
        printf("Left Expected: %d; Left Compute: %d\nRight Expected: %d; Right Compute: %d\n\n", expected.l_speed, compute.l_speed, expected.r_speed, compute.r_speed);
    }

    return 0;
}