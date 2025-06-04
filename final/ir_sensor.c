#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "pid.h"
#include "stdlib.h"

#define DELTA_LARGE 3
#define DELTA_FINE 1
#define RANGE_SERVO 2
#define CENTER_ANGLE 90
#define ROTATION_SPEED 50            // Speed used for in-place turning

// 360° takes 3950 ms → 1 degree takes ~11 ms
#define TURN_TIME_PER_DEGREE 11      // Rounded from 3950 / 360 ≈ 10.97

#define MOVE_SPEED 50                // Forward drive speed
#define MOVE_DURATION_MS 2000       // Duration to move forward after centering
#define SWEEP_TOTAL_DURATION_MS 3230    // Time for full rotation
#define STEP_DELAY_MS 30                // Time between IR samples (can be 20–50ms)
#define ANGLE_INCREMENT (360.0 / (SWEEP_TOTAL_DURATION_MS / STEP_DELAY_MS))  // ~2.73
#define LINE_THRESHOLD 150  // Adjust based on your readings


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

uint8_t get_range(){
    return 165 - analog(ANALOG0_PIN);
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

        degrees = degrees + (direction * DELTA_FINE);

        // read_ir();

        set_servo(2, degrees);

        _delay_ms(5);

    }

}

uint8_t get_closest(){

    u08 degrees = 0;
    u08 range = 0;

    u08 min_range = -1;
    u08 min_range_deg = 0;
    // int8_t direction = 1;
    set_servo(2, 0);

    _delay_ms(250);

    while(degrees <= 180){
        range = get_range();

        if(range < min_range){
            min_range = range;
            min_range_deg = degrees;
        }

        degrees += DELTA_LARGE;
        set_servo(2, degrees);

        clear_screen();
        lcd_cursor(0,0);
        print_string("Degrees:");
        lcd_cursor(0, 1);
        print_num(degrees);

        lcd_cursor(5, 1);
        print_num(range);

        _delay_ms(30);
    }

    
    
    
    // typecasts to signed int, returns degrees from center
    // return ((int8_t) min_range_deg - 90);  
    return min_range_deg;

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


u16 getLeft(){
    return analog(ANALOG4_PIN);
}

u16 getRight(){
    return analog(ANALOG5_PIN);
}

int on_line(){

    return (getLeft() > LINE_THRESHOLD || getRight() > LINE_THRESHOLD);
}
// void track(){
//     int8_t closest_deg = 0;
//     u08 closest_range = 0;
//     int8_t dif_center = 0;
//     int8_t dif_old = 0; 
//     u08 cur_range = 0;
//     u08 cur_degree = 0;
//     int8_t dir = 1;

//     while(1){
//         closest_deg = get_closest();
//         cur_degree = closest_deg;
//         set_servo(RANGE_SERVO, closest_deg);

//         closest_range = analog(ANALOG0_PIN);

//         while(on_line() == 0){
//             cur_range = analog(ANALOG0_PIN);

//             // if current reading is larger, switch swivel directions
//             if(cur_range > closest_range){
//               dir *= -1;  
              
//             } 

//             // continue to sweep motor
//             cur_degree += dir;
//             set_servo(RANGE_SERVO, closest_deg);

//             dif_center = cur_degree - 90;

//             // PID Controller that takes in difference from center and adjusts motors accordingly
//             pid(dif_center, dif_old);
//             dif_old = dif_center;
//         }

//     }

    


// }


void turn_in_place_to_center(uint8_t closest_angle) {
    int total_delay = closest_angle * TURN_TIME_PER_DEGREE;

    // Always rotate clockwise (left wheel forward, right wheel backward)
    motor(0, ROTATION_SPEED);
    motor(1, -ROTATION_SPEED);

    // Delay loop for the needed duration
    for (int i = 0; i < total_delay; i++) {
        _delay_ms(1);  // Fine-grained control
    }

    // Stop motors
    motor(0, 0);
    motor(1, 0);

    // Recenter IR sensor
    set_servo(RANGE_SERVO, 90);
    _delay_ms(100);
}


void drive_forward() {
    motor(0, MOVE_SPEED);
    motor(1, MOVE_SPEED);

    while(!on_line()) {
        _delay_ms(50);  // Check every 50 ms
    }

    motor(0, 0);
    motor(1, 0);
}

uint8_t sweep_for_closest() {
    uint8_t degrees, best_angle = 90;
    uint16_t raw;
    float distance, min_distance = 255;

    for (degrees = 0; degrees <= 180; degrees += 5) {
        set_servo(RANGE_SERVO, degrees);
        _delay_ms(20);

        raw = analog(ANALOG0_PIN);
        distance = (179.0 - raw) / 5.75;
        if (distance < 3.0) distance = 3.0;
        if (distance > 21.0) distance = 21.0;

        if (distance < min_distance) {
            min_distance = distance;
            best_angle = degrees;
        }
    }

    return best_angle;
}
uint8_t smooth_rotate_and_find_closest() {
    float current_angle = 0.0;
    float best_angle = 0.0;
    float min_distance = 255.0;
    uint16_t raw;
    float distance;

    // Start rotating smoothly
    motor(0, ROTATION_SPEED);
    motor(1, -ROTATION_SPEED);

    for (int t = 0; t < SWEEP_TOTAL_DURATION_MS; t += STEP_DELAY_MS) {
        raw = analog(ANALOG0_PIN);
        distance = (179.0 - raw) / 5.75;

        if (distance < 3.0) distance = 3.0;
        if (distance > 21.0) distance = 21.0;

        if (distance < min_distance) {
            min_distance = distance;
            best_angle = current_angle;
        }

        // --- LCD Display during rotation ---
        clear_screen();
        lcd_cursor(0, 0);
        print_string("A:");                   // Current angle
        print_num((int)current_angle);        // Cast to int for display

        lcd_cursor(0, 1);
        print_string("R:");
        print_num(raw);                       // Raw ADC reading

        _delay_ms(STEP_DELAY_MS);
        current_angle += ANGLE_INCREMENT;
    }

    // Stop rotating
    motor(0, 0);
    motor(1, 0);

    // --- LCD Display after rotation ---
    clear_screen(); 
    lcd_cursor(0, 0);
    print_string("B");
    lcd_cursor(0, 1);
    print_num((uint8_t)best_angle);

    return (uint8_t)best_angle;
}


void test_full_rotation_time() {
    motor(0, ROTATION_SPEED);   // Left wheel forward
    motor(1, -ROTATION_SPEED);  // Right wheel backward

    // Let it rotate one full turn
    _delay_ms(3950);  // Start with 1s as a guess, adjust as needed

    motor(0, 0);
    motor(1, 0);
}


int main(){
    init();
    init_adc();
    init_motor();

    motor(1, 0);
    motor(0, 0);

    // int16_t degrees = 0;

    // degrees = get_closest();

    // _delay_ms(20);
    // // set_servo(2, degrees);
     set_servo(2, 90);
    // _delay_ms(200);

    // clear_screen();
    // lcd_cursor(0,0);
    // print_string("Final:");
    // lcd_cursor(0, 1);
    // print_num(degrees);
    //track();
   // Step 1: Initial sweep
    //uint8_t closest_angle = sweep_for_closest();

    // while (1) {
    //     uint8_t closest = sweep_for_closest();
    //     turn_in_place_to_center(closest);
    //     drive_forward(MOVE_DURATION_MS);
    // }
    //test_full_rotation_time();
    //smooth_rotate_and_find_closest();
     while (1) {
        uint8_t closest_angle = smooth_rotate_and_find_closest();
        _delay_ms(500);  // Short delay to stabilize after turning
        turn_in_place_to_center(closest_angle-10);  // Rotate back to target, adjust argument to center robot to can
        _delay_ms(500);  // Short delay to stabilize after turning
        drive_forward();         // Push can
    }
    
    
}