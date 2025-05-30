#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define DELTA_LARGE 3
#define DELTA_FINE 1
#define RANGE_SERVO 2

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

void read_ir(){

    int analog1 = analog(ANALOG0_PIN);

    clear_screen();
    lcd_cursor(0,0);
    print_string("Range:");
    lcd_cursor(0, 1);
    print_num(180 - analog1);

    _delay_ms(10);

}

void swivel_motor(){
    int degrees = 0;
    int direction = 1;

    while(1){
        if(degrees <= 5){
            direction =1;
        }
        else if (degrees >= 180){
            direction = -1;
        }

        degrees = degrees + (direction * DEGREE_CHANGE);

        // read_ir();

        clear_screen();
        lcd_cursor(0,0);
        print_string("Degrees:");
        lcd_cursor(0, 1);
        print_num(degrees);

        set_servo(2, 90);

        // _delay_ms(10);

    }

}

int8_t get_closest(){

    u08 degrees = 0;
    u08 range = 0;

    u08 min_range = -1;
    u08 min_range_deg = 0;
    // int8_t direction = 1;

    while(degrees <= 180){
        range = analog(ANALOG0_PIN);

        if(range < min_range){
            min_range = range;
            min_range_deg = degrees;
        }

        degrees += DELTA_LARGE;
    }

    // typecasts to signed int, returns degrees from center
    return ((int8_t) degrees - 90);   

}

/*
Brainstorm:
- initial sweep, finds the closest object and starts tracking
    - tracking involves slightly moving the sensor left and right
    - if the object is closer on the left or right, move the robot that direction

- while the sensor tracks, the robot uses a PID controller to follow the sensor

- all of this is in a while loop
    - 1 forever loop (or until button press to reset)
        - 1 inside loop that goes until the light sensors detect edge, then turn around and repeat


*/

void on_line(){
    return false;
}

void track(){
    int8_t closest_deg = 0;
    u08 closest_range = 0;
    int8_t dif_center = 0;
    int8_t dif_old = 0; 
    u08 cur_range = 0;
    u08 cur_degree = 0;
    int8_t dir = 1;

    while(1){
        closest_deg = get_closest();
        cur_degree = closest_deg;
        set_servo(RANGE_SERVO, closest_deg);

        closest_range = analog(ANALOG0_PIN);

        while(!on_line()){
            cur_range = analog(ANALOG0_PIN);

            // if current reading is larger, switch swivel directions
            if(cur_range > closest_range){
              dir *= -1;  
              
            } 

            // continue to sweep motor
            cur_degree += dir;
            set_servo(RANGE_SERVO, closest_deg);

            dif_center = cur_degree - 90

            // PID Controller that takes in difference from center and adjusts motors accordingly
            motor_pid(dif_center, dif_old);
            dif_old = dif_center;
        }

    }



}



int main(){
    init();
    init_adc();
    init_motor();

    motor(1, 0);
    motor(0, 0);

    swivel_motor();
}