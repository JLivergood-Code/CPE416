#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

/*
    - brainstorming
        - use motor function
        - light sensor -> reads in value from 0 -> 255
        - sends light sensor readings to controller
        - controller responds with values from 0 -> 100 for motors
            - proportional maps 255 -> 100
            - derivative makes it run smoothly
                - implemented by taking difference in readings
            - integral tunes program
                - implemented with sum of last 5


*/

// CHANGE DEPENDING ON MAT
#define CALIBRATION 100
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

u16 getLeft(){
    return analog(ANALOG1_PIN);
}

u16 getRight(){
    return analog(ANALOG0_PIN);
}

void lineFollow(){
    u08 lServoPos;
    u08 rServoPos;

    u16 lSensor, rSensor;

    float lError, rError, lErrorOld, rErrorOld;

    float kP = (100/255);
    float kD = 1; 
    float kI = 0;
    
    float pLeft, dLeft, iLeft;
    float pRight, dRight, iRight;

    lErrorOld = 0;
    rErrorOld = 0;

    while(1){
        //======================================================================//
        // LEFT MOTOR
        lSensor = getLeft();
        lError -= CALIBRATION;
        
        pLeft = kP * lError;
        dLeft = kD * (lError - lErrorOld);
        lErrorOld = lError;

        lServoPos = pLeft + dleft;
        set_servo(LEFT_MOTOR, lServoPos);
        

        // ======================================================================//
        // RIGHT

        rSensor = getRight();
        rError -= Calibration;
    }
}

int main(){
    init();
    init_adc();

    while(1){
        lineFollow();
    }

    return 0;
}