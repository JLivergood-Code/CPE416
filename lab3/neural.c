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

typedef struct {
    int numInput;
    float inputs[MAX_NODES];
    float weights[MAX_NODES];
    float bias;
    float output;
} Node;

void initialize_network(Node *outNodes, Node *hiddenNodes){

    u08 nodeI;
    u08 weightI;

    for(nodeI = 0; nodeI < NUM_INPUTS; nodeI++){
        for(weightI = 0; weightI < NUM_INPUTS; weightI++){
            hiddenNodes[nodeI].weights[weightI] = rand() / RAND_MAX;
        }
    }
    
    for(nodeI = 0; nodeI < OUT_NODES; nodeI++){
        for(weightI = 0; weightI < HIDDEN_NODES; weightI++){
            outNodes[nodeI].weights[weightI] = rand() / RAND_MAX;
        }
    }
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

float calculate_hidden_value(float target, float out, float input, float connect_weight, float output_node){
    float dout;
    float doutput_tot; // references values calculated for output nodes
    float doutput_net; // references values calculated for output nodes
    float dnet;
    float dweight;
    
    doutput_net = output_node * (1 - output_node);
    doutput_tot = output_node - target;

    dout = doutput_net * doutput_tot * connect_weight;
    dnet = out * (1 - out);
    dweight = input; 

    return dout * dnet * dweight;
}

struct motor_command caclulate_motors(Node *outNodes, Node *hiddenNodes){
    
}

void neural_network(u08 numEpochs){
    /*
        data structure:
        - list of out nodes
        - list of in nodes
        - each list has a node 
    */
    Node outNodes[OUT_NODES];
    Node hiddenNodes[HIDDEN_NODES];

    Node outNew[OUT_NODES];
    Node hiddenNew[HIDDEN_NODES];

    struct motor_command motor_vals;
    u08 trainingI = 0;
    u08 nodeI;

    float alpha = 0.3;

    // STEP 1: Initialize Neural Network

    initialize_network(outNodes, hiddenNodes);

    // STEP 2: Train
    for(trainingI = 0; trainingI < numEpochs; trainingI++){
        // STEP 3: isolate each parameter, starting with output nodes
        // This includes all weights and all biases
        for(nodeI = 0; nodeI < OUT_NODES; nodeI++){
            // iterate through each value in output node weights[]
        }
        // STEP 4, repeat for hidden Nodes
        for(nodeI = 0; nodeI < HIDDEN_NODES; nodeI++){
            // need connecting output node "output" value
        }
    }

}

int main(){
    init();
    init_adc();
    init_lcd();

    neural_network(10);

    return 0;
}