#include <stdio.h>
#include "ev3api.h"
#include "sorts.h"

#ifndef _H_MY_STEERING_
#define _H_MY_STEERING_


void steering_start(ports_t *ster, int power, int steering)
{

    float right_power;
    float left_power;

    int st = steering;
    //ステアリング定義域
    if (100 < steering)
    {
        st = 100;
    }
    else if (steering < -100)
    {
        st = -100;
    }

    if (st == 0)
    {
        right_power = power * (ster -> right_motor_fix);
        left_power = power * (ster -> left_motor_fix);
    }
    else if (st > 0)
    {
        left_power = power * (ster -> left_motor_fix);
        right_power = (power - ( (power / 50.00) * steering) ) * (ster -> right_motor_fix);
    }
    else
    {
        right_power = power * (ster -> right_motor_fix);
        left_power = (power + ( (power / 50.00) * steering) ) * (ster -> left_motor_fix);
    }

    //モーター最大値
    if (right_power > 100)
    {
        right_power = 100;
    }
    else if (right_power < -100)
    {
        right_power = -100;
    }
    if (left_power > 100)
    {
        left_power = 100;
    }
    else if (left_power < -100)
    {
        left_power = -100;
    }
    
    ev3_motor_set_power(ster -> right_motor_port,(int)(right_power));
    ev3_motor_set_power(ster -> left_motor_port,(int)(left_power));

}

#endif