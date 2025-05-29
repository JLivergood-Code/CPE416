#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

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
    const int change = 3;

    while(1){
        if(degrees <= 5){
            direction =1;
        }
        else if (degrees >= 180){
            direction = -1;
        }

        degrees = degrees + (direction * change);

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

int main(){
    init();
    init_adc();
    init_motor();

    motor(1, 0);
    motor(0, 0);

    swivel_motor();
}