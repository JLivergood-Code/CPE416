#include "pid.h"

int main(){
    init();
    init_adc();
    init_lcd();

    struct motor_command motor_val;
    u08 lSensor;
    u08 rSensor;
    u08 lSensor_old = 0;
    u08 rSensor_old = 0;
    u08 lPosition;
    u08 rPosition;

    while(1){

        lSensor = getLeft();
        rSensor = getRight();

        motor_val = compute_proportional(lSensor, rSensor, lSensor_old, rSensor_old);
        lSensor_old = lSensor;
        rSensor_old = rSensor;

        lPosition = motor(0, motor_val.l_speed);
        rPosition = motor(1, motor_val.r_speed);
        display_motor(motor_val, lSensor, rSensor);

        _delay_ms(10); 

    }

    return 0;
}