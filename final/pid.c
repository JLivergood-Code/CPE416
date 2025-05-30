#include "pid.h"

struct motor_command pid(int8_t angle_err, int8_t err_old){
    struct motor_command motor_val;
    int  rError;
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
    dRight = kD * (rError - (0 - err_old));


    motor_val.l_speed = pLeft + dLeft + CALIBRATION_L;
    motor_val.r_speed = pRight + dRight + CALIBRATION_R;

    return motor_val;
}