#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

struct motor_command compute_proportional(uint8_t left, uint8_t right);

void motor(uint8_t num, int8_t speed) {
    if (num == 1) {
        speed = -speed;
    }
    int servo_position = 0.3 * speed + 127;
    if (servo_position < 97) {
        servo_position = 97;
    }
    if (servo_position > 157) {
        servo_position = 157;
    }
    set_servo(num, servo_position);
}

struct motor_command compute_proportional(uint8_t left, uint8_t right){
    int lError, rError, lErrorOld, rErrorOld;
    int error;

    float kP = 0.1;
    float kD = 0.02; 
    // float kI = 0;
    
    float pLeft, dLeft; //, iLeft;
    float pRight, dRight; //, iRight;

    lError = lSensor - rSensor; // + CALIBRATION_L;
        
    pLeft = kP * lError;
    dLeft = kD * (lError - lErrorOld);
    lErrorOld = lError;

    
    

    // ======================================================================//
    // RIGHT

    rError = lSensor - rSensor; // + CALIBRATION_R;

    pRight = kP * rError;
    dRight = kD * (rError - rErrorOld);
    rErrorOld = rError;


    // IF BOTH detect white, turn one otor faster than the others

    lServoPos = pLeft + dLeft + CALIBRATION_L;
    rServoPos = pRight + dRight + CALIBRATION_R;
}

