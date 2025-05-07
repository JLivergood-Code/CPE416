/*
Names: Joshua Livergood and Danny Moreno
Lab3, part 1 
PID controller for expected values
*/

#include "pid.h"

#define CALIBRATION_L 15
#define CALIBRATION_R 15


u08 getLeft(){
    return analog(ANALOG0_PIN);
}

u08 getRight(){
    return analog(ANALOG1_PIN);
}

int motor(uint8_t num, int8_t speed) {
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

    return servo_position;
}

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

void display_motor(struct motor_command in_motor, u08 lPos, u08 rPos){
    char bufferR[8];
    char bufferL[8];
    
    clear_screen();
    snprintf(bufferL, 8, "%d:%d", in_motor.l_speed, lPos);
    snprintf(bufferR, 8, "%d:%d", in_motor.r_speed, rPos);
    lcd_cursor(0,0);
    print_string(bufferL);
    lcd_cursor(0,1);
    print_string(bufferR);

}

