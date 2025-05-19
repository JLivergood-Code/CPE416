#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void run_encoder();

void init_encoder() {
    // enable encoder interrupts
    EIMSK = 0;
    EIMSK |= _BV(PCIE1) | _BV(PCIE0);
    PCMSK1 |= _BV(PCINT13); //PB5 - digital 5
    PCMSK0 |= _BV(PCINT6);  //PE6 - digital 4
    // enable pullups
    PORTE |= _BV(PE6);
    PORTB |= _BV(PB5);
}


volatile uint16_t left_encoder = 0;
volatile uint16_t right_encoder = 0;


ISR(PCINT0_vect) {
   left_encoder++;  //increment left encoder
}

ISR(PCINT1_vect) {
   right_encoder++;  //increment right encoder
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

void run_encoder(){
    
    lcd_cursor(0, 0);
    print_string("Tick:");
    // while(1){
    //     lcd_cursor(0, 1);
    //     print_num(left_encoder);
    //     motor(0, 20); //left
    //     motor(1, 0);//right

    // }
     while(left_encoder < 100 ){//run until 100 ticks
        lcd_cursor(0, 1);
        print_num(left_encoder);
        motor(0, 20); //left
        motor(1, 20);//right
        
    }
    //stop motors after 100 ticks
     motor(0, 0); //left
     motor(1, 0);//right
}

int main(void) {
    init();  //initialize board hardware
    init_encoder(); //initialize encoder
    run_encoder(); //run encoder
    set_servo(0, 140);
    return 0;
}