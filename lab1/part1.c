#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void LED_fade(void) {
    while(1){
        led_on(1);
        _delay_ms(1);  
        led_off(1);
        _delay_ms(20);  
        led_on(1);
        _delay_ms(2);
        led_off(1);
        _delay_ms(19);  
        led_on(1);
        _delay_ms(3);
        led_off(1);
        _delay_ms(18);  
        led_on(1);
        _delay_ms(4);
        led_off(1);
        _delay_ms(17);  
        led_on(1);
        _delay_ms(5);
        led_off(1);
        _delay_ms(16);
    
        
        
        led_on(1);
        _delay_ms(500);
    
    
        led_off(1);
        _delay_ms(1);
        led_on(1);
        _delay_ms(20);
        led_off(1);
        _delay_ms(2);
        led_on(1);
        _delay_ms(19); 
        led_off(1);
        _delay_ms(3);
        led_on(1);
        _delay_ms(18); 
        led_off(1);
        _delay_ms(4);
        led_on(1);
        _delay_ms(17);
        led_off(1);
        led_on(1);
        _delay_ms(16); 
        led_off(1);
        _delay_ms(5);
        led_on(1);
        _delay_ms(15); 
        led_off(1);
        _delay_ms(6);
        led_on(1);
        _delay_ms(14); 
        led_off(1);
        _delay_ms(7);
        led_on(1);
        _delay_ms(13); 
        led_off(1);
        _delay_ms(8);
    }
    
    
 }

int main(void) {
   init();  //initialize board hardware
   LED_fade();  //fade LED1
   return 0;
}


