#include "ev3api.h"
#include "steering.h"

#ifndef _H_MY_ONE_LINETRASE_
#define _H_MY_ONE_LINETRASE_

void one_linetrase(ports_t *port, sensor_port_t sensor, int line, int goal, int speed, float p_gain);

void one_linetrase(ports_t *port, sensor_port_t sensor, int line, int goal, int speed, float p_gain)
{

    int diff = goal - ev3_color_sensor_get_reflect(sensor);
    steering_start(port,speed,(diff * line * p_gain));

}

void one_linetrase_pid(ports_t *port,sensor_port_t sensor,int line,int goal,int speed,float p_gain,float d_gain)
{
    int diff = goal - (int)ev3_color_sensor_get_reflect(sensor);
    
    int ster = (diff * p_gain);

    if (port -> linetrase_fst)
    {
        port -> past_ster = ster;
        port ->linetrase_fst = 0;
    }
    port -> steering = line * (ster + ((ster - port -> past_ster) * d_gain));//変更
    port -> log = (ster - port -> past_ster) * d_gain;
    steering_start(port,speed,(int)port -> steering);
    port -> past_ster = ster;
}

void one_line_cm(ports_t *port, sensor_port_t sensor, int line, int goal, int speed, float p_gain,float cm)
{
    int fst_right = ev3_motor_get_counts(port -> right_motor_port);
    int fst_left = ev3_motor_get_counts(port -> left_motor_port);
    float now = (((float)(abs(ev3_motor_get_counts(port -> right_motor_port) - fst_right)) * (float)(port -> dir_convert_val)) + ((float)(abs(ev3_motor_get_counts(port -> left_motor_port) - fst_left)) * (float)(port -> dir_convert_val))) / 2.0;

    while (now < cm)
    {
        one_linetrase(port,sensor,line,goal,speed,p_gain);
        now = (((float)(abs(ev3_motor_get_counts(port -> right_motor_port) - fst_right)) * (float)(port -> dir_convert_val)) + ((float)(abs(ev3_motor_get_counts(port -> left_motor_port) - fst_left)) * (float)(port -> dir_convert_val))) / 2.0;
    }
}

#endif