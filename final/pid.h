#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define CALIBRATION_L 20
#define CALIBRATION_R 20

struct motor_command{
    uint8_t l_speed;
    uint8_t r_speed;
};


struct motor_command pid(int8_t angle_err, int8_t err_old);

        