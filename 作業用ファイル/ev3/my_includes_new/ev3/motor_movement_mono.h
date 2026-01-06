#include "ev3api.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
#include "sorts.h"

#ifndef _H_MOTOR_MOVEMENTS_
#define _H_MOTOR_MOVEMENTS_


typedef uint32_t micro_sec_t;
typedef int degree;

#define _MY_PI_ 3.1416
#define _DEFAULT_SIN_WAIT_ 0.05
#define _DEFAULT_SIN_TIME_ 8

typedef enum {
    NORMAL_START = 0, //加減速なし
    SIN_START = 1, //三角関数加減速
    PRO_START = 2, //比例加減速
} start_situation_t;

typedef enum {
    NORMAL_END = 0,
    SIN_END = 1,
    PRO_END = 2,
} end_situation_t;


//関数のプロトタイプ宣言
void mv__motor_start(ports_t port, int power);
void mv__motor_stop(ports_t port);
void mv__motor_start_sin(ports_t port, int power, micro_sec_t wait, int times);
void mv__motor_stop_sin(ports_t port, micro_sec_t waittime, int times);
void mv__motor_start_pro(ports_t port, int power, micro_sec_t wait, int times);
void mv__motor_stop_pro(ports_t port, micro_sec_t wait, int times);


//モーター開始関数
void mv__motor_start(ports_t port, int power)
{
    ev3_motor_set_power(port,power);
}

//モーター停止関数
void mv__motor_stop(ports_t port)
{
    ev3_motor_set_power(port,0);
}

//モーター三角開始関数
void mv__motor_start_sin(ports_t port, int power, micro_sec_t wait, int times)
{

    ev3_motor_set_power(port,power);

    float radian;
    for (int i = 0; i < times; i++)
    {

        radian = sin((_MY_PI_ / 2.0) * (((float)i + 1.0000) / (float)times));
        ev3_motor_set_power(port,(int)((float)power * radian));

        tslp_tsk(wait);
    }

    ev3_motor_set_power(port,power);

}

//モーター三角関数停止関数
void mv__motor_stop_sin(ports_t port, micro_sec_t waittime, int times)
{
    int fst = ev3_motor_get_power(port);
    float sin_val;

    for (int i = 0; i < times; i++)
    {
        sin_val = sin((_MY_PI_ / 2) * ((float)(times - i) / times));
        ev3_motor_set_power(port,((int)(fst * sin_val)));

        tslp_tsk(waittime);
    }

    mv__motor_stop(port);
    
}

//モーター比例開始関数
void mv__motor_start_pro(ports_t port, int power, micro_sec_t wait, int times)
{
    float pro = 0;
    for (int i = 1;i < times + 1;i++)
    {
        pro = (float)i / times;
        ev3_motor_set_power(port,power * pro);

        tslp_tsk(wait);
    }
    ev3_motor_set_power(port,power);
}

//モーター比例停止関数
void mv__motor_stop_pro(ports_t port, micro_sec_t wait, int times)
{
    float pro = 0;
    int fst = ev3_motor_get_power(port);
    for (int i = 1;i < times + 1;i++)
    {
        pro = (float)i / times;
        ev3_motor_set_power(port,fst * (1.0 - pro));

        tslp_tsk(wait);
    }
    ev3_motor_set_power(motors -> right_motor_port,0);
}

//以下より未制作

//モーター秒数間
void mv__motor_run_time(ports_t *motors,int right_power, int left_power, micro_sec_t time,start_situation_t start,end_situation_t end)
{

    if (start == SIN_START)
    {
        mv__motor_start_sin(motors,right_power,left_power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (start == NORMAL_START)
    {
        ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_fix_sign);
        ev3_motor_set_power(motors -> left_motor_port,left_power * motors -> left_fix_sign);

        tslp_tsk(_DEFAULT_SIN_TIME_ * _DEFAULT_SIN_WAIT_ * 1000 * 1000);
    }
    else if (start == PRO_START)
    {
        mv__motor_start_pro(motors,right_power,left_power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }

    tslp_tsk(time - (_DEFAULT_SIN_TIME_ * _DEFAULT_SIN_WAIT_ * 2.0 * 1000 * 1000));

    if (end == SIN_END)
    {
        mv__motor_stop_sin(motors,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (end == NORMAL_END)
    {
        mv__motor_stop(motors);

        tslp_tsk(_DEFAULT_SIN_TIME_ * _DEFAULT_SIN_WAIT_ * 1000 * 1000);
    }
    else if (end == PRO_END)
    {
        mv__motor_stop_pro(motors,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }


}

//モーターcm間
#define _START_SIN_ 0.6 // 0 < _START_SIN_ < 1
#define _GAIN_ 0.2
mv__motor_run_cm(ports_t *motors, int power,float cm, start_situation_t start,end_situation_t stop)
{
    int first_right = ev3_motor_get_counts(motors -> right_motor_port);
    int first_left = ev3_motor_get_counts(motors -> left_motor_port);
    int goal = cm * (motors -> dis_convert_val);
    if (start == SIN_START)
    {
        mv__motor_start_sin(motors,power,power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (start == NORMAL_START)
    {
        mv__motor_start(motors,power,power);
    }
    else if (start == PRO_START)
    {
        mv__motor_start_pro(motors,power,power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    int ave;
    if (stop == SIN_END || stop == PRO_END)
    {
        ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        while(ave < goal * _START_SIN_)
        {
            mv__motor_start(motors,power,power);
            ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        }
        while (ave < goal)
        {
            if (stop == SIN_END)
            {
                float s = cos((ave - goal * _START_SIN_) / (goal * (1.0 - _START_SIN_)) * (_MY_PI_ / 2)) + _GAIN_;
                if (1.0 < s)
                {
                    s = 1.0;
                }
                mv__motor_start(motors,(int)(power * s),(int)(power * s));
            }
            else
            {
                float p = (ave - goal * _START_SIN_) / (goal * (1.0 - _START_SIN_));
                if (1.0 < p)
                {
                    p = 1.0;
                }
                mv__motor_start(motors,(int)(power * p),(int)(power * p));
            }
            
            ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        }
    }
    else
    {
        ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        while(ave < goal)
        {
            mv__motor_start(motors,power,power);
            ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        }
    }

    mv__motor_stop(motors);
}


#endif
