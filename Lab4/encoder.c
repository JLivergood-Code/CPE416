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

void turn(int8_t side, u08 degrees){
    // if side == 1, robot turns right,if side == -1, robot turns left
    int8_t left_dir = 1 * side; 
    int8_t right_dir = -1 * side;

    u08 num_ticks = 59;

    num_ticks = num_ticks * ((float) degrees / 180) + 2;

    left_encoder = 0;
    right_encoder = 0;

    while(left_encoder < num_ticks && right_encoder < num_ticks){
        motor(0, 20*left_dir);
        motor(1, 20*right_dir);

        lcd_cursor(0, 1);
        print_num(left_encoder);

    }

    motor(0, 0);
    motor(1, 0);

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

    for(u08 i = 0; i < 4; i++){
        u08 num_ticks = 108;

        left_encoder = right_encoder = 0;

        while(left_encoder < num_ticks && right_encoder < num_ticks ){//run until 100 ticks
            lcd_cursor(0, 1);
            print_num(left_encoder);
            lcd_cursor(5, 1);
            print_num(right_encoder);
            motor(0, 20); //left
            motor(1, 20);//right
            
        }
        left_encoder = right_encoder = 0;
        //stop motors after 100 ticks

        //turning stuff
        turn(-1, 90);
    }
}



void run_encoder_bowtie(){
    
    lcd_cursor(0, 0);
    print_string("Tick:");
    // while(1){
    //     lcd_cursor(0, 1);
    //     print_num(left_encoder);
    //     motor(0, 20); //left
    //     motor(1, 0);//right

    // }
    u08 num_ticks = 108;
    u08 num_ticks_diag = 155;
    while((left_encoder < num_ticks) && (right_encoder < num_ticks )){//run until 100 ticks
            lcd_cursor(0, 1);
            print_num(left_encoder);
            lcd_cursor(5, 1);
            print_num(right_encoder);
            motor(0, 20); //left
            motor(1, 20);//right
            
        }
    left_encoder = right_encoder = 0;
    clear_screen();
    turn(-1, 140);
    left_encoder = right_encoder = 0;
    while((left_encoder < num_ticks_diag) && (right_encoder < num_ticks_diag )){//run until 100 ticks
            lcd_cursor(0, 1);
            print_num(left_encoder);
            lcd_cursor(5, 1);
            print_num(right_encoder);
            motor(0, 20); //left
            motor(1, 20);//right
            
        }
    left_encoder = right_encoder = 0;
    clear_screen();
    turn(1, 137);
    left_encoder = right_encoder = 0;
    while((left_encoder < num_ticks) && (right_encoder < num_ticks) ){//run until 100 ticks
            lcd_cursor(0, 1);
            print_num(left_encoder);
            lcd_cursor(5, 1);
            print_num(right_encoder);
            motor(0, 20); //left
            motor(1, 20);//right
            
        }
    left_encoder = right_encoder = 0;
    clear_screen();
    turn(1, 137);
    left_encoder = right_encoder = 0;
    while((left_encoder < num_ticks_diag) && (right_encoder < num_ticks_diag) ){//run until 100 ticks
            lcd_cursor(0, 1);
            print_num(left_encoder);
            lcd_cursor(5, 1);
            print_num(right_encoder);
            motor(0, 20); //left
            motor(1, 20);//right
            
        }
    
    // for(u08 i = 0; i < 4; i++){
    //     u08 num_ticks = 108;

    //     left_encoder = right_encoder = 0;

    //     while(left_encoder < num_ticks && right_encoder < num_ticks ){//run until 100 ticks
    //         lcd_cursor(0, 1);
    //         print_num(left_encoder);
    //         lcd_cursor(5, 1);
    //         print_num(right_encoder);
    //         motor(0, 20); //left
    //         motor(1, 20);//right
            
    //     }
    //     left_encoder = right_encoder = 0;
    //     //stop motors after 100 ticks

    //     //turning stuff
    //     turn(-1, 90);
    // }
}

int main(void) {
    init();  //initialize board hardware
    init_encoder(); //initialize encoder
    
    motor(0, 0);
    motor(1, 0);

    while(get_btn() == 0);


    // int mode = 0; // 0 = square, 1 = bowtie

    // while (1) {
    //     // Wait for button press (debounced)
    //     if (get_btn() == 1) {
    //         while(get_btn() == 1);  // Wait for button release
    //         mode = !mode;           // Toggle between 0 and 1
    //     }

    //     if (mode == 0) {
            
    //         lcd_cursor(0,0);
    //         print_string("Square");
    //         run_encoder();   // Run square pattern
    //     } else {
            
    //         lcd_cursor(0,0);
    //         print_string("Bowtie");
    //         run_encoder_bowtie();   // Run bowtie pattern
    //     }
    // }
// }
//     run_encoder(); //run encoder in square pattern
    
    run_encoder_bowtie(); //run encoder in bowtie pattern
    return 0;
}
