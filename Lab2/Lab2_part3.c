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
#define CALIBRATION 19


#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

u08 getLeft(){
    return analog(ANALOG1_PIN);
}

u08 getRight(){
    return analog(ANALOG0_PIN);
}

void lineFollow(){
    u08 lServoPos;
    u08 rServoPos;

    u08 lSensor, rSensor;

    float lError, rError, lErrorOld, rErrorOld;

    float kP = (30/190);
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
        // error is really large when lsesnor is 0 (on white), and small when lsensor reads 180 (on line)
        lError = (lSensor - rSensor) + CALIBRATION;

        
        pLeft = kP * lError;
        dLeft = kD * (lError - lErrorOld);
        lErrorOld = lError;

        
        

        // ======================================================================//
        // RIGHT

        rSensor = getRight();
        rError = (rSensor - lSensor) + CALIBRATION;

        pRight = kP * rError;
        dRight = kD * (rError - rErrorOld);
        rErrorOld = rError;


        // IF BOTH detect white, turn one otor faster than the others

        lServoPos = pLeft + dLeft;
        rServoPos = pRight + dRight;

        set_servo(LEFT_MOTOR, lServoPos);
        set_servo(RIGHT_MOTOR, rServoPos);
    }
}

int main(){
    init();
    init_adc();

    lineFollow();

    return 0;
}