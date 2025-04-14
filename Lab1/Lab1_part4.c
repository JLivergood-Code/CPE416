#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

char *full_str = "416";

int main(void) {
    init();  // Initialize board hardware

    while (1) {
        clear_screen();
        u08 x = get_accel_x();
        u08 y = get_accel_y();
        static int col = 2; 
        static int row = 0; 

        if (y < 5) {
            //Flat
        } else if (y < 65) {
            if (col > -2) col--; //Tilted left
        } else if (y > 185) {
            if (col < 7) col++; //Tilted right
        } 

        if ((x <= 254 && x >= 250) || (x <= 5)){
            //Flat
        } else if (x < 252 && x > 185) {
            if (row < 1) row++; //Tilted towards 
        } else if (x <= 60) {
            if (row > 0) row--;; //Tilted away
        }

        if (col < 0) {
            lcd_cursor(0, row);
            print_string(&full_str[-col]); 
        } else {
            lcd_cursor(col, row);
            print_string(full_str);
        }
        
        _delay_ms(200); 
    }
}