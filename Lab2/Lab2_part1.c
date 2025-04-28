#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

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

void acceleration(void){
    char buffer[16];
    while (1){
        for (int i = 100; i >= -100; i--){
            motor(0, i); //Decelerate left motor
            motor(1, i); //Decelerate right 
            snprintf(buffer, sizeof(buffer), "%d", i);
            clear_screen();
            lcd_cursor(0, 0);
            print_string("Speed:");
            lcd_cursor(0, 1);
            print_string(buffer);
            _delay_ms(50); 
        }
        for (int i = -100; i <=100; i++){
            motor(0, i); //Accelerate left motor
            motor(1, i); //Accelerate right motor
            snprintf(buffer, sizeof(buffer), "%d", i);
            clear_screen();
            lcd_cursor(0, 0);
            print_string("Speed:");
            lcd_cursor(0, 1);
            print_string(buffer);
            _delay_ms(50); 
        }
        
    }
    
}

int main(void) {
    init();  //initialize board hardware
    lcd_cursor(0, 0); 
    print_string("Speed:");
    acceleration();  

    return 0;
}
