#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "pid.h"

float calculate_out_value(float target, float out1, float h1){

}

float calculate_hidden_value(float target, float out1, float h1){

}

void neural_network(){
    /*
        data structure:
        - list of out nodes
        - list of in nodes
        - each list has a node 
    */
}

int main(){
    init();
    init_adc();
    init_lcd();

    neural_network();

    return 0;
}