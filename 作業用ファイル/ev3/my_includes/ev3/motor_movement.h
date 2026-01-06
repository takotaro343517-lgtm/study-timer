#include "ev3api.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
#include "sorts.h"

#ifndef _H_MOTOR_MOVEMENTS_
#define _H_MOTOR_MOVEMENTS_

#define _PI_ 3.1415926

typedef uint32_t micro_sec_t;
typedef int degree;

//モーター開始
void mv__motor_run(ports_t *motors, int power, int steering)
{
    if (0 < steering)
    {
        ev3_motor_set_power(motors -> right_motor_port,(power - (power / 100) * steering) * motors -> right_motor_fix);
        ev3_motor_set_power(motors -> left_motor_port,power * motors -> left_motor_fix);
    }
    else
    {
        ev3_motor_set_power(motors -> right_motor_port,power * motors -> right_motor_fix);
        ev3_motor_set_power(motors -> left_motor_port,power * (power - (power / 100) * steering) * motors -> left_motor_fix);
    }
}

//モーター三角関数開始
void mv__motor_accelerate(ports_t *motors, int right_power, int left_power, int time, micro_sec_t wait)
{

    int fst_right_power = ev3_motor_get_power(motors -> right_motor_port) * (motors -> right_fix_sign);
    int fst_left_power = ev3_motor_get_power(motors -> left_motor_port) * (motors -> left_fix_sign);

    int right_motor = right_power - fst_right_power;
    int left_motor = left_power - fst_left_power;

    for (int i = 0; i < time; i++)
    {

        double radian = (90 / time * i) * M_PI / 180;
        ev3_motor_set_power((motors -> right_motor_port),((fst_right_power + (right_motor * sin(radian))) * (motors -> right_motor_fix)));
        ev3_motor_set_power((motors -> left_motor_port),((fst_right_power + (left_motor * sin(radian))) * (motors -> left_motor_fix)));

        tslp_tsk(wait);
    }

}

//モーター停止
void mv__motor_stop(ports_t *motors)
{
    ev3_motor_set_power(motors -> right_motor_port,0);
    ev3_motor_set_power(motors -> left_motor_port,0);
}


void mv__motor_run_time(ports_t *motors,int right_power, int left_power, micro_sec_t time)
{

    ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_fix_sign);
    ev3_motor_set_power(motors -> left_motor_port,left_power * motors -> left_fix_sign);

    tslp_tsk(time);

    ev3_motor_set_power(motors -> right_motor_port,0);
    ev3_motor_set_power(motors -> left_motor_port,0);
}


void mv__motor_run_time_sin(ports_t *motors, int right_power, int left_power, unsigned int time, micro_sec_t wait, micro_sec_t waittime)
{

    int fst_right_power = ev3_motor_get_power(motors -> right_motor_port) * (motors -> right_fix_sign);
    int fst_left_power = ev3_motor_get_power(motors -> left_motor_port) * (motors -> left_fix_sign);

    int right_motor = right_power - fst_right_power;
    int left_motor = left_power - fst_left_power;

    for (int i = 0; i < time; i++)
    {

        double radian = (90 / time * i) * M_PI / 180;
        ev3_motor_set_power((motors -> right_motor_port),((fst_right_power + (right_motor * sin(radian))) * (motors -> right_motor_fix)));
        ev3_motor_set_power((motors -> left_motor_port),((fst_right_power + (left_motor * sin(radian))) * (motors -> left_motor_fix)));

        tslp_tsk(wait);
    }

    tslp_tsk(waittime - (time * wait * 2));

    right_motor = ev3_motor_get_power(motors -> right_motor_port) * -1;
    left_motor = ev3_motor_get_power(motors -> left_motor_port) * -1;

    for (int i = 0; i < time; i++)
    {

        double radian = (90 / time * i) * M_PI / 180;
        ev3_motor_set_power((motors -> right_motor_port),((fst_right_power + (right_motor * sin(radian))) * (motors -> right_motor_fix)));
        ev3_motor_set_power((motors -> left_motor_port),((fst_right_power + (left_motor * sin(radian))) * (motors -> left_motor_fix)));

        tslp_tsk(wait);
    }

    ev3_motor_set_power(motors -> right_motor_port,0);
    ev3_motor_set_power(motors -> left_motor_port,0);
}


void mv__motor_run_rotate(ports_t *motors, int right_power, int left_power, double degree)
{

    int fst_right = ev3_motor_get_counts(motors -> right_motor_port);
    int fst_left = ev3_motor_get_counts(motors -> left_motor_port);

    int right_judge = 0;
    int left_judge = 0;


    while (right_judge == 0 || left_judge == 0)
    {
        
        ev3_motor_set_power(motors -> right_motor_port, right_power * (motors -> right_fix_sign));
        ev3_motor_set_power(motors -> left_motor_port, left_power * (motors -> left_fix_sign));
        
        
        if (fst_right + degree * (right_power * (motors -> right_motor_fix) >= 0 ? 1 : -1) <= ev3_motor_get_counts(motors -> right_motor_port))
        {
            right_judge = 1;
        }
        else if (fst_right + degree * (right_power * (motors -> right_motor_fix) >= 0 ? 1 : -1) >= ev3_motor_get_counts(motors -> right_motor_port))
        {
            right_judge = 1;
        }

        if (fst_left + degree * (left_power * (motors -> left_motor_fix) >= 0 ? 1 : -1) <= ev3_motor_get_counts(motors -> left_motor_port))
        {
            left_judge = 1;
        }
        else if (fst_left + degree * (left_power * (motors -> left_motor_fix) >= 0 ? 1 : -1) >= ev3_motor_get_counts(motors -> left_motor_port))
        {
            left_judge = 1;
        }
        
    }

    ev3_motor_set_power(motors -> right_motor_port, 0);
    ev3_motor_set_power(motors -> left_motor_port, 0);
}

void mv__motor_rotate(ports_t *motors, degree angle, int power)
{
    
    int move_to_dir_right;
    int move_to_dir_left;
    int angle_sign = angle >= 0 ? 1 : -1;

    int right_forward = (motors -> right_fix_sign) * angle_sign * power;
    int left_forward = (motors -> left_fix_sign) * angle_sign * power * -1; //ここの-1は回転するとき反対は逆回転になるため

    int aim = abs(((motors -> tire_distance) * angle) / (motors -> tire_diameter));

    if (right_forward >= 0)
    {
        move_to_dir_right = ev3_motor_get_counts(motors -> right_motor_port) + aim;
    }
    else
    {
        move_to_dir_right = ev3_motor_get_counts(motors -> right_motor_port) - aim;
    }

    if (left_forward >= 0)
    {
        move_to_dir_left = ev3_motor_get_counts(motors -> left_motor_port) + aim; 
    }
    else
    {
        move_to_dir_left = ev3_motor_get_counts(motors -> left_motor_port) - aim;
    }

    bool_t loop = true;

    float sin_start = 0.6;
    //sin関数で減速するタイミング(0 ~ 1)

    while (loop)
    {

        int now_aim = aim - ((abs(move_to_dir_right - ev3_motor_get_counts(motors -> right_motor_port)) + abs(move_to_dir_left - ev3_motor_get_counts(motors -> left_motor_port))) / 2);

        if (aim * sin_start <= now_aim)
        {
            double sin_rad = sin((aim - now_aim) / (aim * (1 - sin_start)) * (0.5 * M_PI));
            right_forward = (motors -> right_fix_sign) * angle_sign * power * sin_rad;
            left_forward = (motors -> left_fix_sign) * angle_sign * power * -1 * sin_rad;
        }
        
        ev3_motor_set_power((motors -> right_motor_port), right_forward);
        ev3_motor_set_power((motors -> left_motor_port), left_forward);

        if (right_forward >= 0 && move_to_dir_right <= ev3_motor_get_counts(motors -> right_motor_port))
        {loop = false;}
        else if (right_forward < 0 && move_to_dir_right >= ev3_motor_get_counts(motors -> right_motor_port))
        {loop = false;}

        if (left_forward >= 0 && move_to_dir_left <= ev3_motor_get_counts(motors -> left_motor_port))
        {loop = false;}
        else if (left_forward < 0 && move_to_dir_left >= ev3_motor_get_counts(motors -> left_motor_port))
        {loop = false;}
        
    }
    ev3_motor_set_power(motors -> right_motor_port,0);
    ev3_motor_set_power(motors -> left_motor_port,0);
}

#endif
