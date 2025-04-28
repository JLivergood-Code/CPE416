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
}

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
    struct motor_command motor_val;
    int lError, rError, lErrorOld, rErrorOld;
    int error;

    float kP = 0.1;
    float kD = 0.02; 
    // float kI = 0;
    
    float pLeft, dLeft; //, iLeft;
    float pRight, dRight; //, iRight;

    lError = lSensor - rSensor; 
        
    pLeft = kP * lError;
    dLeft = kD * (lError - lErrorOld);
    lErrorOld = lError;

    // ======================================================================//
    // RIGHT
    // THIS all technically is just a copy of the left motor

    rError = lSensor - rSensor; 

    pRight = kP * rError;
    dRight = kD * (rError - rErrorOld);
    rErrorOld = rError;

    motor_val.l_speed = pLeft + dLeft + CALIBRATION_L;
    motor_val.r_speed = pRight + dRight + CALIBRATION_R;

    return motor_val;
}

void display_motor(struct motor_command in_motor){
    clear_screen();
        snprintf(bufferL, 8, "L:%d", in_motor.l_speed);
        snprintf(bufferR, 8, "R:%d", in_motor.r_speed);
        lcd_cursor(0,0);
        print_string(bufferL);
        lcd_cursor(0,1);
        print_string(bufferR);


}

