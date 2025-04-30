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



u08 getLeft(){
    return analog(ANALOG0_PIN);
}

u08 getRight(){
    return analog(ANALOG1_PIN);
}

void controller(){
    //start in proportional
    int mode = 0; //0 for prop control, 1 for data cap mode, 2 for training, 3 for run neural network
    uint16_t num_epochs;
    while (mode == 0){
        //proportional control
        lcd_cursor(0,0);
        print_string("proport");
        if (get_btn() == 1 && mode == 0) {
            mode = 1; // Switch to data capture
        }
    }
    clear_screen();
    while (1){
        if (mode == 1){
            //data capture
            clear_screen();
            lcd_cursor(0,0);
            print_string("Data");
            int button_pressed = 0;
            //capture data and  store in an array
            int sensor_readings[5000][2];
            int i = 0;
            while(!button_pressed){
                sensor_readings[i][0] = getLeft();
                sensor_readings[i][1] =  getRight();
                i++;
                button_pressed = get_btn();
            }

            
            if (button_pressed == 1 && mode == 1){
                mode = 2;//set mode to training
                
            }
        }
        else if (mode == 2){
            clear_screen();
            lcd_cursor(0,0);
            print_string("Training");
            //get num epochs with accelerometer
            num_epochs;

            neural_network(num_epochs);//call training function with sensor_readings as argument
        }
        else if (mode == 3){
            //neural network running
            clear_screen();
            lcd_cursor(0,0);
            print_string("Follow");
            
            if (get_btn() == 1 && mode == 2){
                mode = 2;//set mode to training
            }
        }
        else{
            //default
        }
    }
}



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

struct motor_command caclulate_motors(Node *outNodes, Node *hiddenNodes){//compute neural network
    
}



void neural_network(u08 numEpochs){//train neural network
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

int main(void) {
    init();        
    init_adc();    
    while(1) {
        controller();
    }
    return 0;
}
