#include "pid.h"

#define CALIBRATION_L 20
#define CALIBRATION_R 20


u08 getLeft(){
    return analog(ANALOG0_PIN);
}

u08 getRight(){
    return analog(ANALOG1_PIN);
}

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
    int lError, rError; // , lErrorOld, rErrorOld;
    //int error;

    float kP = 0.1;
    //float kD = 0.02; 
    // float kI = 0;
    
    float pLeft; // dLeft; //, iLeft;
    float pRight; // dRight; //, iRight;

    lError = left - right; 
        
    pLeft = kP * lError;
    //dLeft = kD * (lError - lErrorOld);
    //lErrorOld = lError;

    // ======================================================================//
    // RIGHT
    // THIS all technically is just a copy of the left motor

    rError = left - right; 

    pRight = kP * rError;
    //dRight = kD * (rError - rErrorOld);
    // rErrorOld = rError;

    motor_val.l_speed = pLeft + CALIBRATION_L;
    motor_val.r_speed = pRight + CALIBRATION_R;

    return motor_val;
}

void display_motor(struct motor_command in_motor){
    char bufferR[8];
    char bufferL[8];
    
    clear_screen();
    snprintf(bufferL, 8, "L:%d", in_motor.l_speed);
    snprintf(bufferR, 8, "R:%d", in_motor.r_speed);
    lcd_cursor(0,0);
    print_string(bufferL);
    lcd_cursor(0,1);
    print_string(bufferR);

}

