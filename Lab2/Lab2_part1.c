#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

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

void acceleration(void){
    while (1){
        for (int i = 0; i <=100 i++){
            motor(0, i); //Accelerate left motor
            motor(1, i); //Accelerate right motor
            print_num(i); //Print speed to LCD
            _delay_ms(10); //Delay to allow for acceleration
        }
        for (int i = 100; i >= -100; i--){
            motor(0, i); //Decelerate left motor
            motor(1, i); //Decelerate right 
            print_num(i); //Print speed to LCD.
            _delay_ms(10); //Delay to allow for deceleration
        }
    }
    
}

void init_servo(){
    //connect pins and stuff
    digital_dir(0, 1); //Set pin 0 to output
    digital_dir(1, 1); //Set pin 1 to output
}

int main(void) {
    init();  //initialize board hardware
    lcd_cursor(0, 0); 
    init_servo();
    acceleration();  

    return 0;
}
