#include "ev3api.h"
#include "app.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
#include "sorts.h"


void linetrase(ports_t *linetrase, int speed, double p_gain, double factorial, double d_gain)
{

    int right_color = ev3_color_sensor_get_reflect(linetrase -> right_sensor_port);
    int left_color = ev3_color_sensor_get_reflect(linetrase -> left_sensor_port);

    int ster_sign;
    if ((left_color - right_color) >= 0)
    {
        ster_sign = 1;
    }
    else
    {
        ster_sign = -1;
    }
    
    float steering = p_gain * (pow(abs(left_color - right_color),factorial) * ster_sign);

    if ((linetrase -> first_time) == 1)
    {
        linetrase -> past_ster = steering;
        linetrase -> first_time = 0;
    }

    signed int r_motor_power = (int)((linetrase -> right_motor_fix) * (speed - steering - ((steering - (linetrase -> past_ster)) * d_gain)));
    signed int l_motor_power = (int)((linetrase -> left_motor_fix) * (speed + steering + ((steering - (linetrase -> past_ster)) * d_gain)));

    linetrase -> past_ster = steering;


    if (r_motor_power > 100)
    {
        r_motor_power = 100;
    }
    else if (r_motor_power < -100)
    {
        r_motor_power = -100;
    }

    if (l_motor_power > 100)
    {
        l_motor_power = 100;
    }
    else if (l_motor_power < -100)
    {
        l_motor_power = -100;
    }

    ev3_motor_set_power((linetrase -> right_motor_port),r_motor_power);
    ev3_motor_set_power((linetrase -> left_motor_port),l_motor_power);

    linetrase -> steering_row = (left_color - right_color);
    linetrase -> steering = steering;
    linetrase -> right_motor_power = r_motor_power;
    linetrase -> left_motor_power = l_motor_power;
}
