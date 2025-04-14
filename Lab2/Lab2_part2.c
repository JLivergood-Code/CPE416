#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define AGGRO 0
#define FEAR 1

#define SENSOR0 0
#define SENSOR1 1

#define LEFT 0
#define RIGHT 1

void motor(uint8_t num, int8_t speed) { //num will be 0 or 1 corresponding to the left or right motor.  speed will be a number from -100 (full speed reverse) to +100 (full speed forward).
    //This function should call set_servo to spin the motor
    //A call to set_servo with position of 127 should stop the motor.  If your motor spins, you can calibrate your motors by adjusting the servo potentiometer with a screwdriver.
    //code to translate speed to servo position
    
    int servo_position = 0.3*speed + 127; //translate speed to servo position
    if (servo_position < 97) {
        servo_position = 97; //Clamp to minimum
    }
    if (servo_position > 157) {
        servo_position = 157; //Clamp to maximum
    }
    if (num == 0) {
        set_servo(0, servo_position); //Set left motor
        
    } else if (num == 1) {
        set_servo(1, servo_position); //Set right motor
    }

}

void main(){
    /* 
    TODO:
        1. setup photo sensor
            remap intputs from 0-255 -> 0-100
        2. setup button press to change which sensor controls which motor
            - switch statement for AGRESSION and FEAR
        3. move motors according to photo sensor values
    
    */

    u08 sensor0Val;
    u08 sensor1Val;

    u08 robotType;


    while(1){
        // IMPLEMENT getSensor()
        sensor0Val = getSensor(SENSOR0);
        sensor1Val = getSensor(SENSOR1);

    }

}