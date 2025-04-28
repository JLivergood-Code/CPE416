#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

/*
    - brainstorming
        - use motor function
        - light sensor -> reads in value from 0 -> 255
        - sends light sensor readings to controller
        - controller responds with values from 0 -> 100 for motors
            - proportional maps 255 -> 100
            - derivative makes it run smoothly
                - implemented by taking difference in readings
            - integral tunes program
                - implemented with sum of last 5


*/

// CHANGE DEPENDING ON MAT
#define CALIBRATION_L 130
#define CALIBRATION_R 138

#define TARGET 190

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

u08 getLeft(){
    return analog(ANALOG0_PIN);
}

u08 getRight(){
    return analog(ANALOG1_PIN);
}

void lineFollow(){
    u08 lServoPos;
    u08 rServoPos;

    u08 lSensor, rSensor;

   	int lError, rError, lErrorOld, rErrorOld;
    int sum = 0;

    float kP = 0.08;
    float kD = 0.002; 
    float kI = 0.02;
    
    float pLeft, dLeft, iLeft;
    float pRight, dRight, iRight;

    char bufferR[8];
    char bufferL[8];

    lErrorOld = 0;
    rErrorOld = 0;

    while(1){
        //======================================================================//
        // LEFT MOTOR
        lSensor = getLeft();
        rSensor = getRight();

        // + if facing right, - if facing left
        //error = lSensor - rSensor;

        // error is really large when lsesnor is 0 (on white), and small when lsensor reads 180 (on line)
       
			 	lError = lSensor - rSensor; // + CALIBRATION_L;
        sum += lError;

				if(sum >= 500) { sum = 500; }
				if(sum <= -500) { sum = -500; }


        pLeft = kP * lError;
        dLeft = kD * (lError - lErrorOld);
				iLeft = kI * sum;
        lErrorOld = lError;

        
        

        // ======================================================================//
        // RIGHT

        rError = lSensor - rSensor; // + CALIBRATION_R;

        pRight = kP * rError;
        dRight = kD * (rError - rErrorOld);
				iRight = kI * sum;
        rErrorOld = rError;


        // IF BOTH detect white, turn one otor faster than the others

        lServoPos = pLeft + dLeft +  CALIBRATION_L;
        rServoPos = pRight + dRight + CALIBRATION_R;

        clear_screen();
        snprintf(bufferL, 8, "%d:%d", lError, lServoPos);
        snprintf(bufferR, 8, "%d:%d", rError, rServoPos);
        lcd_cursor(0,0);
        print_string(bufferL);
        lcd_cursor(0,1);
        print_string(bufferR);

        set_servo(0, lServoPos);
        set_servo(1, rServoPos);

        _delay_ms(10); 
    }
}

int main(){
    init();
    init_adc();
    init_lcd();

    lineFollow();

    return 0;
}
