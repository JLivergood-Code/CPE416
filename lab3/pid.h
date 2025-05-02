#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#ifndef PID_H_
#define PID_H_

struct motor_command{

    uint8_t l_speed;
    uint8_t r_speed;
};

u08 getLeft();
u08 getRight();

struct motor_command compute_proportional(uint8_t left, uint8_t right, u08 left_old, u08 right_old);
int motor(uint8_t num, int8_t speed);
void display_motor(struct motor_command in_motor, u08 lPos, u08 rPos);

#endif