#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>



void braitenberg(void){
    char buffer[16];
    int servo0_position, servo1_position;
    u16 light_0, light_1;
    static u08 mode = 0; // 0 = fear, 1 = aggression
    if (get_btn() == 1 && mode == 0) {
        mode = 1; // Switch to aggression mode
    } else if (get_btn() == 1 && mode == 1) {
        mode = 0; // Switch to fear mode
    }
    if (mode == 0) {//Fear mode
        
        light_0 = analog(ANALOG0_PIN);
        light_1 = analog(ANALOG1_PIN);
        _delay_ms(300);          // Small delay so it's readable
        clear_screen();         // Clear previous content
        lcd_cursor(0, 0);
        print_string("F Mode");
        if(light_0 < light_1) {
            // Move left
            servo0_position = (light_0 - 220) * 30 / (240 - 220) + 127;
            servo1_position = 127 - (light_1 - 220) * 30 / (240 - 220);
            set_servo(0, servo0_position);
            set_servo(1, servo1_position );
            
            // snprintf(buffer, sizeof(buffer), "0 %d", servo0_position);
    
            // // Display it on the LCD
            // clear_screen();         // Clear previous content
            // lcd_cursor(0, 0);        // Top-left of LCD
            // print_string(buffer);
            // snprintf(buffer, sizeof(buffer), "1 %d", servo1_position);
            // lcd_cursor(0,1);        // Bottom-left of LCD
            // print_string(buffer);
            // lcd_cursor(0, 7);
            // print_string("L");
        } else {
            // Move right
            servo0_position = (light_0 - 220) * 30 / (240 - 220) + 127;
            servo1_position = 127 - (light_1 * 30) / 255;
            set_servo(0, servo0_position);
            set_servo(1, servo1_position);
            // snprintf(buffer, sizeof(buffer), "0 %d", servo0_position);
    
            // // Display it on the LCD
            // clear_screen();         // Clear previous content
            // lcd_cursor(0, 0);        // Top-left of LCD
            // print_string(buffer);
            // snprintf(buffer, sizeof(buffer), "1 %d", servo1_position);
            // lcd_cursor(0,1);        // Bottom-left of LCD
            // print_string(buffer);
            // lcd_cursor(0, 7);
            // print_string("R");
        }
    } else if (mode == 1) {
        light_0 = analog(ANALOG0_PIN);
        light_1 = analog(ANALOG1_PIN);
        _delay_ms(300);          // Small delay so it's readable
        clear_screen();         // Clear previous content
            lcd_cursor(0, 0);
            print_string("A Mode");
        if(light_0 > light_1) {
            // Move left
            servo0_position = (light_0 - 220) * 30 / (240 - 220) + 127;
            servo1_position = 127 - (light_1 - 220) * 30 / (240 - 220);
            set_servo(0, servo0_position);
            set_servo(1, servo1_position );
            
            // snprintf(buffer, sizeof(buffer), "0 %d", servo0_position);
    
            // // Display it on the LCD
            // clear_screen();         // Clear previous content
            // lcd_cursor(0, 0);        // Top-left of LCD
            // print_string(buffer);
            // snprintf(buffer, sizeof(buffer), "1 %d", servo1_position);
            // lcd_cursor(0,1);        // Bottom-left of LCD
            // print_string(buffer);
            // lcd_cursor(0, 7);
            // print_string("L");
        } else {
            // Move right
            servo0_position = (light_0 - 220) * 30 / (240 - 220) + 127;
            servo1_position = 127 - (light_1 * 30) / 255;
            set_servo(0, servo0_position);
            set_servo(1, servo1_position);
            // snprintf(buffer, sizeof(buffer), "0 %d", servo0_position);
    
            // // Display it on the LCD
            // clear_screen();         // Clear previous content
            // lcd_cursor(0, 0);        // Top-left of LCD
            // print_string(buffer);
            // snprintf(buffer, sizeof(buffer), "1 %d", servo1_position);
            // lcd_cursor(0,1);        // Bottom-left of LCD
            // print_string(buffer);
            // lcd_cursor(0, 7);
            // print_string("R");
            
        }
    }
}

void fear(void) {
    char buffer[16];
    int servo0_position, servo1_position;
    u16 light_0, light_1;
    u08 button_pressed = 0; // Initialize button_pressed to 0 (fear mode)
    while(button_pressed == 0){
        light_0 = analog(ANALOG0_PIN);
        light_1 = analog(ANALOG1_PIN);
        _delay_ms(300);          // Small delay so it's readable
        
        if(light_0 > light_1) {
            // Move left
            servo0_position = (light_0 - 220) * 30 / (240 - 220) + 127;
            servo1_position = 127 - (light_1 - 220) * 30 / (240 - 220);
            set_servo(0, servo0_position);
            set_servo(1, servo1_position );
            
            snprintf(buffer, sizeof(buffer), "0 %d", servo0_position);
    
            // Display it on the LCD
            clear_screen();         // Clear previous content
            lcd_cursor(0, 0);        // Top-left of LCD
            print_string(buffer);
            snprintf(buffer, sizeof(buffer), "1 %d", servo1_position);
            lcd_cursor(0,1);        // Bottom-left of LCD
            print_string(buffer);
            lcd_cursor(0, 7);
            print_string("L");
        } else {
            // Move right
            servo0_position = (light_0 - 220) * 30 / (240 - 220) + 127;
            servo1_position = 127 - (light_1 * 30) / 255;
            set_servo(0, servo0_position);
            set_servo(1, servo1_position);
            snprintf(buffer, sizeof(buffer), "0 %d", servo0_position);
    
            // Display it on the LCD
            clear_screen();         // Clear previous content
            lcd_cursor(0, 0);        // Top-left of LCD
            print_string(buffer);
            snprintf(buffer, sizeof(buffer), "1 %d", servo1_position);
            lcd_cursor(0,1);        // Bottom-left of LCD
            print_string(buffer);
            lcd_cursor(0, 7);
            print_string("R");
        }
    }
    
}

void aggression(void) {
    char buffer[16];
    int servo0_position, servo1_position;
    u16 light_0, light_1;
    u08 button_pressed = 0; // Initialize button_pressed to 0 (fear mode)
    while(button_pressed == 0){
        light_0 = analog(ANALOG0_PIN);
        light_1 = analog(ANALOG1_PIN);
        _delay_ms(300);          // Small delay so it's readable
        
        if(light_0 < light_1) {
            // Move left
            servo0_position = (light_0 - 220) * 30 / (240 - 220) + 127;
            servo1_position = 127 - (light_1 - 220) * 30 / (240 - 220);
            set_servo(0, servo0_position);
            set_servo(1, servo1_position );
            
            snprintf(buffer, sizeof(buffer), "0 %d", servo0_position);
    
            // Display it on the LCD
            clear_screen();         // Clear previous content
            lcd_cursor(0, 0);        // Top-left of LCD
            print_string(buffer);
            snprintf(buffer, sizeof(buffer), "1 %d", servo1_position);
            lcd_cursor(0,1);        // Bottom-left of LCD
            print_string(buffer);
            lcd_cursor(0, 7);
            print_string("L");
        } else {
            // Move right
            servo0_position = (light_0 - 220) * 30 / (240 - 220) + 127;
            servo1_position = 127 - (light_1 * 30) / 255;
            set_servo(0, servo0_position);
            set_servo(1, servo1_position);
            snprintf(buffer, sizeof(buffer), "0 %d", servo0_position);
    
            // Display it on the LCD
            clear_screen();         // Clear previous content
            lcd_cursor(0, 0);        // Top-left of LCD
            print_string(buffer);
            snprintf(buffer, sizeof(buffer), "1 %d", servo1_position);
            lcd_cursor(0,1);        // Bottom-left of LCD
            print_string(buffer);
            lcd_cursor(0, 7);
            print_string("R");
        }
    }
    
}

int main(void) {
    init();        
    init_adc();    
    //fear();
    //aggression();
    while(1) {
        braitenberg();
    }
    
   
    return 0;
}
