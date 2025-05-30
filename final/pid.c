#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define CALIBRATION_L 20
#define CALIBRATION_R 20

struct motor_command{
    uint8_t l_speed;
    uint8_t r_speed;
};

struct motor_command pid(int8_t angle_err, int8_t err_old){
    struct motor_command motor_val;
    int lError, rError, lErrorOld, rErrorOld;
    //int error;

    float kP = 0.055;
    float kD = 0.008; 
    // float kI = 0;
    
    float pLeft, dLeft; //, iLeft;
    float pRight, dRight; //, iRight;

    // left motor        
    pLeft = kP * angle_err;
    dLeft = kD * (angle_err - err_old);

    // right motor
    rError = 0 - angle_err; 
    pRight = kP * rError;
    dRight = kD * (rError - rErrorOld);


    motor_val.l_speed = pLeft + dLeft + CALIBRATION_L;
    motor_val.r_speed = pRight + dRight + CALIBRATION_R;

    return motor_val;
}