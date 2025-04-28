/*
Names: Joshua Livergood and Danny Moreno
Lab1, part 1
Causes an LED to blink using PWM
*/



#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void LED_fade(void) {
    while (1) {
        //Fade In
        for (int brightness = 0; brightness <= 20; brightness++) {
            led_on(1);
            for (int i = 0; i < brightness; i++) {
                _delay_ms(1); //Time that LED is on
            }
            led_off(1);
            for (int i = 0; i < (20 - brightness); i++) {
                _delay_ms(1); //Time that LED is off
            }
        }
        //Stay on for a moment
        led_on(1);
        _delay_ms(500);
        led_off(1);

        //Fade Out
        for (int brightness = 20; brightness >= 0; brightness--) {
            led_on(1);
            for (int i = 0; i < brightness; i++) {
                _delay_ms(1); //Time that LED is on
            }
            led_off(1);
            for (int i = 0; i < (20 - brightness); i++) {
                _delay_ms(1); //Time that LED is off
            }
        }

        //Stay off for a moment
        _delay_ms(500);
    }
}

int main(void) {
    init();  //initialize board hardware
    LED_fade();  //Fade LED1
    return 0;
}
