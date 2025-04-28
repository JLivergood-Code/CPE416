#include "pid.h"

int main(){
    init();
    init_adc();
    init_lcd();

    struct motor_command motor_val;
    u08 lSensor;
    u08 rSensor;

    while(1){

        lSensor = getLeft();
        rSensor = getRight();

        motor_val = compute_proportional(lSensor, rSensor);

        set_motor(0, motor_val.l_speed);
        set_motor(1, motor_val.r_speed);

    }

    return 0;
}