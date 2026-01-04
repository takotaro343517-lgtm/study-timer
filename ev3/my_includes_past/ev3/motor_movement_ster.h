#include "ev3api.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
#include "sorts.h"
#include "../useful/clamp.h"

#ifndef _H_MOTOR_MOVEMENTS_
#define _H_MOTOR_MOVEMENTS_


typedef uint32_t micro_sec_t;
typedef unsigned int degree;

#define _MY_PI_ 3.1416
#define _DEFAULT_SIN_WAIT_ 5 * 1000
#define _DEFAULT_SIN_TIME_ 50

typedef enum {
    NORMAL_START = 0, //加減速なし
    SIN_START = 1, //三角関数加減速
    PRO_START = 2, //比例加減速
} start_situation_t;

typedef enum {
    NORMAL_END = 0,
    SIN_END = 1,
    PRO_END = 2,
    NONE_END = 3,
} end_situation_t;


//関数のプロトタイプ宣言
void mv__motors_start(ports_t *motors ,int right_power, int left_power);
void mv__motors_stop(ports_t *motors);
void mv__motors_start_sin(ports_t *motors, int right_power, int left_power, micro_sec_t wait, int times);
void mv__motors_stop_sin(ports_t *motors, micro_sec_t waittime, int times);
void mv__motors_start_pro(ports_t *motors, int right_power, int left_power, micro_sec_t wait, int times);
void mv__motors_stop_pro(ports_t *motors,micro_sec_t wait, int times);


//モーター開始関数
void mv__motors_start(ports_t *motors ,int right_power, int left_power)
{
    ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_motor_fix);
    ev3_motor_set_power(motors -> left_motor_port, left_power * motors -> left_motor_fix);
}

//モーター停止関数
void mv__motors_stop(ports_t *motors)
{
    ev3_motor_stop(motors -> right_motor_port,true);
    ev3_motor_stop(motors -> left_motor_port,true);
}

//モーター三角開始関数
void mv__motors_start_sin(ports_t *motors, int right_power, int left_power, micro_sec_t wait, int times)
{

    ev3_motor_set_power(motors -> right_motor_port,0);
    ev3_motor_set_power(motors ->  left_motor_port,0);

    float sin_val;
    for (int i = 0; i < times; i++)
    {

        sin_val = sin((_MY_PI_ / 2.0) * (((float)i + 1.0000) / (float)times));
        ev3_motor_set_power((motors -> right_motor_port),(int)(((float)right_power * sin_val) * (float)(motors -> right_motor_fix)));
        ev3_motor_set_power((motors -> left_motor_port),(int)(((float)left_power * sin_val) * (float)(motors -> left_motor_fix)));

        tslp_tsk(wait);
    }

    ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_fix_sign);
    ev3_motor_set_power(motors -> left_motor_port,left_power * motors -> left_fix_sign);

}

//モーター三角関数停止関数
void mv__motors_stop_sin(ports_t *motors, micro_sec_t waittime, int times)
{
    int fst_right = ev3_motor_get_power(motors -> right_motor_port);
    int fst_left = ev3_motor_get_power(motors -> left_motor_port);
    float sin_val;

    for (int i = 0; i < times; i++)
    {
        sin_val = sin((_MY_PI_ / 2) * ((float)(times - i) / times));
        ev3_motor_set_power(motors -> right_motor_port,((int)(fst_right * sin_val)));
        ev3_motor_set_power(motors -> left_motor_port,((int)(fst_left * sin_val)));

        tslp_tsk(waittime);
    }

    mv__motors_stop(motors);
    
}

//モーター比例開始関数
void mv__motors_start_pro(ports_t *motors,int right_power,int left_power,micro_sec_t wait, int times)
{
    ev3_motor_set_power(motors -> right_motor_port,0);
    ev3_motor_set_power(motors ->  left_motor_port,0);

    float pro = 0;
    for (int i = 1;i < times + 1;i++)
    {
        pro = (float)i / times;
        ev3_motor_set_power(motors -> right_motor_port,right_power * pro * motors -> right_motor_fix);
        ev3_motor_set_power(motors -> left_motor_port,left_power * pro * motors -> left_motor_fix);

        tslp_tsk(wait);
    }
    ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_motor_fix);
    ev3_motor_set_power(motors -> left_motor_port,left_power * motors -> left_motor_fix);
}

void mv__motors_start_curve_pd(ports_t *motors,int right_power,int left_power,micro_sec_t wait, int times,float pgain,float dgain)
{
    ev3_motor_set_power(motors -> right_motor_port,0);
    ev3_motor_set_power(motors ->  left_motor_port,0);

    float pro = 0;
    float r_diff;
    float l_diff;
    float past_r_diff = 0;
    float past_l_diff = 0;
    float final_pro;
    for (int i = 1;i < times + 1;i++)
    {
        pro = pow((float)i / times,2.0);

        r_diff = (pro - fabs(right_power / ev3_motor_get_power(motors -> right_motor_port)));
        final_pro = pro +  r_diff * pgain + (r_diff - past_r_diff) * dgain;
        ev3_motor_set_power(motors -> right_motor_port,right_power * final_pro * motors -> right_motor_fix);
        past_r_diff = r_diff;

        l_diff = (pro - fabs(left_power / ev3_motor_get_power(motors -> left_motor_port)));
        final_pro = pro +  l_diff * pgain + (l_diff - past_l_diff) * dgain;
        ev3_motor_set_power(motors -> left_motor_port,left_power * final_pro * motors -> right_motor_fix);
        past_l_diff = l_diff;

        tslp_tsk(wait);
    }
    ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_motor_fix);
    ev3_motor_set_power(motors -> left_motor_port,left_power * motors -> left_motor_fix);
}


//モーター比例停止関数
void mv__motors_stop_pro(ports_t *motors,micro_sec_t wait, int times)
{
    float pro = 0;
    int fst_right = ev3_motor_get_power(motors -> right_motor_port);
    int fst_left = ev3_motor_get_power(motors -> left_motor_port);
    for (int i = 1;i < times + 1;i++)
    {
        pro = (float)i / times;
        ev3_motor_set_power(motors -> right_motor_port,fst_right * (1.0 - pro));
        ev3_motor_set_power(motors -> left_motor_port,fst_left * (1.0 - pro));

        tslp_tsk(wait);
    }
    mv__motors_stop(motors);
}

void mv__motors_stop_curve_pd(ports_t *motors,micro_sec_t wait, int times,float pgain,float dgain)
{
    float pro = 0;
    int fst_right = ev3_motor_get_power(motors -> right_motor_port);
    int fst_left = ev3_motor_get_power(motors -> left_motor_port);
    float r_diff;
    float l_diff;
    float past_r_diff = 0;
    float past_l_diff = 0;
    float final_pro;
    for (int i = 1;i < times + 1;i++)
    {
        pro = pow((float)(times - i) / times,2.0);

        r_diff = (pro - fabs(fst_right / ev3_motor_get_power(motors -> right_motor_port)));
        final_pro = pro +  r_diff * pgain + (r_diff - past_r_diff) * dgain;
        ev3_motor_set_power(motors -> right_motor_port,fst_right * final_pro);
        past_r_diff = r_diff;

        l_diff = (pro - fabs(fst_left / ev3_motor_get_power(motors -> left_motor_port)));
        final_pro = pro +  l_diff * pgain + (l_diff - past_l_diff) * dgain;
        ev3_motor_set_power(motors -> left_motor_port,fst_left * final_pro);
        past_l_diff = l_diff;

        tslp_tsk(wait);
    }
    mv__motors_stop(motors);
}

//モーター秒数間
void mv__motors_run_time(ports_t *motors,int right_power, int left_power, micro_sec_t time,start_situation_t start,end_situation_t end)
{

    if (start == SIN_START)
    {
        mv__motors_start_sin(motors,right_power,left_power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (start == NORMAL_START)
    {
        ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_motor_fix);
        ev3_motor_set_power(motors -> left_motor_port,left_power * motors -> left_motor_fix);

        tslp_tsk(_DEFAULT_SIN_TIME_ * _DEFAULT_SIN_WAIT_);
    }
    else if (start == PRO_START)
    {
        mv__motors_start_pro(motors,right_power,left_power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }

    tslp_tsk(time - (_DEFAULT_SIN_TIME_ * _DEFAULT_SIN_WAIT_ * 2.0));

    if (end == SIN_END)
    {
        mv__motors_stop_sin(motors,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (end == NORMAL_END)
    { 
        tslp_tsk(_DEFAULT_SIN_TIME_ * _DEFAULT_SIN_WAIT_);
        mv__motors_stop(motors);
    }
    else if (end == PRO_END)
    {
        mv__motors_stop_pro(motors,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (end == NONE_END)
    {
        
    }


}

//モーターcm間
#define _START_SIN_ 0.6 // 0 < _START_SIN_ < 1
#define _GAIN_ 0.1
mv__motors_run_cm(ports_t *motors, int power,float cm, start_situation_t start,end_situation_t stop)
{
    int first_right = ev3_motor_get_counts(motors -> right_motor_port);
    int first_left = ev3_motor_get_counts(motors -> left_motor_port);
    int goal = cm * (motors -> dis_convert_val);
    if (start == SIN_START)
    {
        mv__motors_start_sin(motors,power,power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (start == NORMAL_START)
    {
        mv__motors_start(motors,power,power);
    }
    else if (start == PRO_START)
    {
        mv__motors_start_pro(motors,power,power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    int ave;
    int change = (int)(goal * _START_SIN_);
    if (stop == SIN_END || stop == PRO_END)
    {
        ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        while(ave < change)
        {
            mv__motors_start(motors,power,power);
            ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        }
        while (ave < goal)
        {
            if (stop == SIN_END)
            {
                float s = cos((ave - change) / (goal * (1.0 - _START_SIN_)) * (_MY_PI_ / 2)) + _GAIN_;
                if (1.0 < s)
                {
                    s = 1.0;
                }
                mv__motors_start(motors,(int)(power * s),(int)(power * s));
            }
            else if (stop == PRO_END)
            {
                float p = (1.0 - (ave - change) / (goal * (1.0 - _START_SIN_))) + _GAIN_;
                if (1.0 < p)
                {
                    p = 1.0;
                }
                mv__motors_start(motors,(int)(power * p),(int)(power * p));
            }
            
            ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        }
    }
    else if (stop == NORMAL_END)
    {
        ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        while(ave < goal)
        {
            mv__motors_start(motors,power,power);
            ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        }
    }

    if (!(stop == NONE_END))
    {
        mv__motors_stop(motors);
    }
    
}


mv__motors_rotate(ports_t *motors, unsigned int power,degree deg, start_situation_t start,end_situation_t stop)
{

    int first_right = ev3_motor_get_counts(motors -> right_motor_port);
    int first_left = ev3_motor_get_counts(motors -> left_motor_port);
    int goal = motors -> tire_distance * abs(deg) / motors -> tire_diameter;
    int right_p = (deg > 0) ? power : -power;
    int left_p = (deg > 0) ? -power : power;


    if (start == SIN_START)
    {
        mv__motors_start_sin(motors,right_p,left_p,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (start == NORMAL_START)
    {
        mv__motors_start(motors,right_p,left_p);
    }
    else if (start == PRO_START)
    {
        mv__motors_start_pro(motors,right_p,left_p,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    int ave;
    if (stop == SIN_END || stop == PRO_END)
    {
        ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        while(ave < goal * _START_SIN_)
        {
            mv__motors_start(motors,right_p,left_p);
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
                mv__motors_start(motors,(int)(right_p * s),(int)(left_p * s));
            }
            else if (stop == PRO_END)
            {
                float p = 1.0 - (ave - goal * _START_SIN_) / (goal * (1.0 - _START_SIN_)) + _GAIN_;
                if (1.0 < p)
                {
                    p = 1.0;
                }
                mv__motors_start(motors,(int)(right_p * p),(int)(left_p * p));
            }
            
            ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        }
    }
    else
    {
        ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        while(ave < goal)
        {
            mv__motors_start(motors,right_p,left_p);
            ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        }
    }

    mv__motors_stop(motors);
}

//モーター軸回転
#define _JIKU_SIN_START_ 0.6
#define _JIKU_GAIN_ 0.2
void mv__motors_jiku_rotate(ports_t *ports, int direction, int degree, int power,start_situation_t start,end_situation_t end)
{

    int aim = (2 * (ports -> tire_distance) * abs(degree)) / (ports -> tire_diameter);

    int judge;
    int fst;
    motor_port_t motor;
    float fix;

    judge = (0 < degree) ? 1 : -1;
    
    fst = (0 < direction) ? ev3_motor_get_counts(ports -> left_motor_port) : ev3_motor_get_counts(ports -> right_motor_port);
    if (0 < direction)
    {
        motor = ports -> left_motor_port;
        fix = ports -> left_motor_fix;
        if (start == SIN_START)
        {
            mv__motors_start_sin(ports,0,power * judge,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
        }
        else if (start == PRO_START)
        {
            mv__motors_start_pro(ports,0,power * judge,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
        }
        else if(start == NORMAL_START)
        {
            mv__motors_start(ports,0,power * judge);
        }
        mv__motors_start(ports,0,power * judge);
        
    }
    else
    {
        motor = ports -> right_motor_port;
        fix = ports -> right_motor_fix;
        if (start == SIN_START)
        {
            mv__motors_start_sin(ports,power * judge,0,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
        }
        else if (start == PRO_START)
        {
            mv__motors_start_pro(ports,power * judge,0,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
        }
        else if(start == NORMAL_START)
        {
            mv__motors_start(ports,power * judge,0);
        }
        mv__motors_start(ports,power * judge,0);
    }

    int gap = abs(ev3_motor_get_counts(motor) - fst);
    int change = aim * _JIKU_SIN_START_;
    if (end == SIN_END || end == PRO_END)
    {
        while(gap < change)
        {
            ev3_motor_set_power(motor,power * judge * fix);
            gap = abs(ev3_motor_get_counts(motor) - fst);
        }
        while(gap < aim)
        {
            
            if (end == SIN_END)
            {
                float s = ((gap - aim * _JIKU_SIN_START_) / (aim * (1.0 - _JIKU_SIN_START_)));
                s = cos((s * (_MY_PI_ / 2))) + _JIKU_GAIN_;
                if (1.0 < s)
                {
                    s = 1.0;
                }
                ev3_motor_set_power(motor,(int)(power * s * judge * fix));
            }
            else if (end == PRO_END)
            {
                float p = 1.0 - (gap - aim * _JIKU_SIN_START_) / (aim * (1.0 - _JIKU_SIN_START_)) + _JIKU_GAIN_;
                if (1.0 < p)
                {
                    p = 1.0;
                }
                ev3_motor_set_power(motor,(int)(power * p * judge * fix));
            }
            gap = abs(ev3_motor_get_counts(motor) - fst);
            
        }
    }
    else if (end == NORMAL_END)
    {
        while (gap < aim)
        {
            ev3_motor_set_power(motor,power * judge * fix);
            gap = abs(ev3_motor_get_counts(motor) - fst);
        }

    }
    
    mv__motors_stop(ports);
    
}

void mv__motors_turn_run(ports_t *motors,int power,uint8_t dir,uint16_t black,float pgain,float dgain,micro_sec_t wait,int times)
{
    /*
    if (dir == 1)
    {
        while(ev3_color_sensor_get_reflect(motors -> right_motor_port > black)
        {
            mv__motors_start(motors,-power,power);
        }
    else
    {
        while(ev3_color_sensor_get_reflect(motors -> left_motor_port > black)
        {
            mv__motors_start(motors,power,-power);
        }
    }
    */
    int diff;
    int p;
    int past = ev3_color_sensor_get_reflect(motors -> right_sensor_port) - ev3_color_sensor_get_reflect(motors -> left_sensor_port);
    for (int i = 0;i < times;i++)
    {
        diff = ev3_color_sensor_get_reflect(motors -> right_sensor_port) - ev3_color_sensor_get_reflect(motors -> left_sensor_port);
        p = diff * pgain - (diff - past) * dgain;

        //p = (p > power) ? power : p;
        //p = (p < -power) ? -power : p;
        mv__motors_start(motors,p,-p);

        past = diff;

        tslp_tsk(wait);
    }
    mv__motors_stop(motors);
}


#define _TIMES_ 70
#define _WAIT_ 6000
#define _STOP_ 0.7
#define _STOP_GAIN_ 8.0
void mv__motors_cm_new(ports_t *ports,float cm,int power,float p_gain,float d_gain)
{
    int fst_right = ev3_motor_get_counts(ports -> right_motor_port);
    int fst_left = ev3_motor_get_counts(ports -> left_motor_port);
    int count_diff = (abs(ev3_motor_get_counts(ports -> right_motor_port) - fst_right) + abs(ev3_motor_get_counts(ports -> left_motor_port) - fst_left)) / 2;
    int goal = cm * (ports -> dis_convert_val);
    int i = 0;
    float pro;
    float final_pro;
    float diff;
    int right_power;
    int left_power;
    float past_right_diff = 0;
    float past_left_diff = 0;
    float decrease = goal * _STOP_;

    //加速
    while (i < _TIMES_ && count_diff < decrease)
    {
        right_power = abs(ev3_motor_get_power(ports -> right_motor_port));
        left_power = abs(ev3_motor_get_power(ports -> left_motor_port));
        count_diff = (abs(ev3_motor_get_counts(ports -> right_motor_port) - fst_right) + abs(ev3_motor_get_counts(ports -> left_motor_port) - fst_left)) / 2;
        pro = pow((float)i / _TIMES_,2.0);

        diff = ((pro) - fabs((float)(right_power) / power));
        final_pro = pro +  diff * p_gain + (diff - past_right_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        ev3_motor_set_power(ports -> right_motor_port,power * final_pro * ports -> right_motor_fix);
        past_right_diff = diff;
        

        diff = ((pro) - fabs((float)(left_power) / power));
        final_pro = pro +  diff * p_gain + (diff - past_left_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        ev3_motor_set_power(ports -> left_motor_port,power * final_pro * ports -> left_motor_fix);
        past_left_diff = diff;
        i += 1;
        tslp_tsk(_WAIT_);
    }
    ev3_motor_set_power(ports -> right_motor_port,power * ((float)i / _TIMES_) * ports -> right_motor_fix);
    ev3_motor_set_power(ports -> left_motor_port,power * ((float)i / _TIMES_) * ports -> left_motor_fix);

    //平坦
    while (count_diff < decrease)
    {
        count_diff = (abs(ev3_motor_get_counts(ports -> right_motor_port) - fst_right) + abs(ev3_motor_get_counts(ports -> left_motor_port) - fst_left)) / 2;
        tslp_tsk(_WAIT_);
    }
    ev3_led_set_color(LED_ORANGE);
    //減速
    int fst_right_power = (ev3_motor_get_power(ports -> right_motor_port));
    int fst_left_power = (ev3_motor_get_power(ports -> left_motor_port));
    
    past_right_diff = 0;    
    past_left_diff = 0;

    while (count_diff < goal)
    {
        right_power = abs(ev3_motor_get_power(ports -> right_motor_port));
        left_power = abs(ev3_motor_get_power(ports -> left_motor_port));
        count_diff = (abs(ev3_motor_get_counts(ports -> right_motor_port) - fst_right) + abs(ev3_motor_get_counts(ports -> left_motor_port) - fst_left)) / 2;

        pro = pow((1.0 - ((count_diff - decrease) / (goal * (1.0 - _STOP_)))),2.0);
        pro = clamp_float(pro,0,1);

        //右モーター
        diff = (((pro) - fabs((float)right_power / fst_right_power)));
        final_pro = pro +  diff * p_gain + (diff - past_right_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        if (fabs(fst_right_power * final_pro) < _STOP_GAIN_)
        {
            if (fst_right_power > 0)
            {
                ev3_motor_set_power(ports -> right_motor_port,_STOP_GAIN_);
            }
            else
            {
                ev3_motor_set_power(ports -> right_motor_port,-_STOP_GAIN_);
            }
        }
        else
        {
            ev3_motor_set_power(ports -> right_motor_port,fst_right_power * final_pro);
        }
        past_right_diff = diff;

        //左モーター
        diff = (((pro) - fabs((float)left_power / fst_left_power)));
        final_pro = pro +  diff * p_gain + (diff - past_left_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        if (fabs(fst_left_power * final_pro) < _STOP_GAIN_)
        {
            if (fst_left_power > 0)
            {
                ev3_motor_set_power(ports -> left_motor_port,_STOP_GAIN_);
            }
            else
            {
                ev3_motor_set_power(ports -> left_motor_port,-_STOP_GAIN_);
            }
        }
        else
        {
            ev3_motor_set_power(ports -> left_motor_port,fst_left_power * final_pro);
        }
        past_left_diff = diff;

        tslp_tsk(_WAIT_);
    }
    mv__motors_stop(ports);
}

void mv__motors_rotate_new(ports_t *ports,degree angle,int power,float p_gain,float d_gain)
{
    int fst_right = ev3_motor_get_counts(ports -> right_motor_port);
    int fst_left = ev3_motor_get_counts(ports -> left_motor_port);
    int count_diff = (abs(ev3_motor_get_counts(ports -> right_motor_port) - fst_right) + abs(ev3_motor_get_counts(ports -> left_motor_port) - fst_left)) / 2;
    int goal = (ports -> tire_distance) * _PI_ * (angle / 360.0) * ports -> dis_convert_val;
    int i = 0;
    float pro;
    float final_pro;
    float diff;
    int right_power;
    int left_power;
    float past_right_diff = 0;
    float past_left_diff = 0;
    float decrease = goal * _STOP_;

    //加速
    while (i < _TIMES_ && count_diff < decrease)
    {
        right_power = abs(ev3_motor_get_power(ports -> right_motor_port));
        left_power = abs(ev3_motor_get_power(ports -> left_motor_port));
        count_diff = (abs(ev3_motor_get_counts(ports -> right_motor_port) - fst_right) + abs(ev3_motor_get_counts(ports -> left_motor_port) - fst_left)) / 2;
        pro = pow((float)i / _TIMES_,2.0);

        diff = ((pro) - fabs((float)(right_power) / power));
        final_pro = pro +  diff * p_gain + (diff - past_right_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        ev3_motor_set_power(ports -> right_motor_port,power * final_pro * ports -> right_motor_fix);
        past_right_diff = diff;
        

        diff = ((pro) - fabs((float)(left_power) / power));
        final_pro = pro +  diff * p_gain + (diff - past_left_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        ev3_motor_set_power(ports -> left_motor_port,-power * final_pro * ports -> left_motor_fix);
        past_left_diff = diff;
        i += 1;
        tslp_tsk(_WAIT_);
    }
    ev3_motor_set_power(ports -> right_motor_port,power * ((float)i / _TIMES_) * ports -> right_motor_fix);
    ev3_motor_set_power(ports -> left_motor_port,-power * ((float)i / _TIMES_) * ports -> left_motor_fix);

    //平坦
    while (count_diff < decrease)
    {
        count_diff = (abs(ev3_motor_get_counts(ports -> right_motor_port) - fst_right) + abs(ev3_motor_get_counts(ports -> left_motor_port) - fst_left)) / 2;
        tslp_tsk(_WAIT_);
    }
    ev3_led_set_color(LED_ORANGE);
    //減速
    int fst_right_power = (ev3_motor_get_power(ports -> right_motor_port));
    int fst_left_power = (ev3_motor_get_power(ports -> left_motor_port));
    
    past_right_diff = 0;    
    past_left_diff = 0;

    while (count_diff < goal)
    {
        right_power = abs(ev3_motor_get_power(ports -> right_motor_port));
        left_power = abs(ev3_motor_get_power(ports -> left_motor_port));
        count_diff = (abs(ev3_motor_get_counts(ports -> right_motor_port) - fst_right) + abs(ev3_motor_get_counts(ports -> left_motor_port) - fst_left)) / 2;

        pro = pow((1.0 - ((count_diff - decrease) / (goal * (1.0 - _STOP_)))),2.0);
        pro = clamp_float(pro,0,1);

        //右モーター
        diff = (((pro) - fabs((float)right_power / fst_right_power)));
        final_pro = pro +  diff * p_gain + (diff - past_right_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        if (fabs(fst_right_power * final_pro) < _STOP_GAIN_)
        {
            if (fst_right_power > 0)
            {
                ev3_motor_set_power(ports -> right_motor_port,_STOP_GAIN_);
            }
            else
            {
                ev3_motor_set_power(ports -> right_motor_port,-_STOP_GAIN_);
            }
        }
        else
        {
            ev3_motor_set_power(ports -> right_motor_port,fst_right_power * final_pro);
        }
        past_right_diff = diff;

        //左モーター
        diff = (((pro) - fabs((float)left_power / fst_left_power)));
        final_pro = pro +  diff * p_gain + (diff - past_left_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        if (fabs(fst_left_power * final_pro) < _STOP_GAIN_)
        {
            if (fst_left_power > 0)
            {
                ev3_motor_set_power(ports -> left_motor_port,_STOP_GAIN_);
            }
            else
            {
                ev3_motor_set_power(ports -> left_motor_port,-_STOP_GAIN_);
            }
        }
        else
        {
            ev3_motor_set_power(ports -> left_motor_port,fst_left_power * final_pro);
        }
        past_left_diff = diff;

        tslp_tsk(_WAIT_);
    }
    mv__motors_stop(ports);
}

void mv__motors_jiku_new(ports_t *ports,uint8_t jiku,degree angle,int power,float p_gain,float d_gain)
{
    
    motor_port_t change = (jiku == 1) ? ports -> left_motor_port : ports -> right_motor_port;
    float fix_val = (jiku == 1) ? ports -> left_motor_fix : ports -> right_motor_fix;
    int fst = ev3_motor_get_counts(change);
    int count_diff = abs(ev3_motor_get_counts(change) - fst);
    int goal = 2.0 * (ports -> tire_distance) * _PI_ * (angle / 360.0) * ports -> dis_convert_val;
    int i = 0;
    float pro;
    float final_pro;
    float diff;
    int now_power;
    float past_diff = 0;
    float decrease = goal * _STOP_;

    //加速
    while (i < _TIMES_ && count_diff < decrease)
    {
        now_power = ev3_motor_get_power(change);
        count_diff = abs(ev3_motor_get_counts(change) - fst);
        pro = pow((float)i / _TIMES_,2.0);

        diff = ((pro) - fabs((float)(now_power) / power));
        final_pro = pro +  diff * p_gain + (diff - past_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        ev3_motor_set_power(change,power * final_pro * fix_val);
        past_diff = diff;
        
        i += 1;
        tslp_tsk(_WAIT_);
    }
    ev3_motor_set_power(change,power * ((float)i / _TIMES_) * fix_val);

    //平坦
    while (count_diff < decrease)
    {
        count_diff = abs(ev3_motor_get_counts(change) - fst);
        tslp_tsk(_WAIT_);
    }
    ev3_led_set_color(LED_ORANGE);
    //減速
    int fst_power = (ev3_motor_get_power(change));
    
    past_diff = 0;

    while (count_diff < goal)
    {
        now_power = abs(ev3_motor_get_power(change));
        count_diff = abs(ev3_motor_get_counts(change) - fst);

        pro = pow((1.0 - ((count_diff - decrease) / (goal * (1.0 - _STOP_)))),2.0);
        pro = clamp_float(pro,0,1);

        
        diff = (((pro) - fabs((float)now_power / fst_power)));
        final_pro = pro +  diff * p_gain + (diff - past_diff) * d_gain;
        final_pro = clamp_float(final_pro,0,1);
        if (fabs(fst_power * final_pro) < _STOP_GAIN_)
        {
            if (fst_power > 0)
            {
                ev3_motor_set_power(change,_STOP_GAIN_);
            }
            else
            {
                ev3_motor_set_power(change,-_STOP_GAIN_);
            }
        }
        else
        {
            ev3_motor_set_power(change,fst_power * final_pro);
        }
        past_diff = diff;

        

        tslp_tsk(_WAIT_);
    }
    mv__motors_stop(ports);
}


//モーターcm待機
void mv__motors_cm_wait(ports_t *port,uint16_t set_fst,int fst, motor_port_t motor, float cm)
{
    int _fst;
    if (set_fst == 1)
    {
        _fst = fst;
    }
    else
    {
        _fst = ev3_motor_get_counts(motor);
    }
    
    while (abs(ev3_motor_get_counts(motor) - _fst) < cm * port -> dis_convert_val)
    {
        tslp_tsk(1000 * 10);
    }
    

}

void mv__motors_run_rotate(ports_t *motors, int right_power, int left_power, double degree, int stop)
{

    int fst_right = ev3_motor_get_counts(motors -> right_motor_port);
    int fst_left = ev3_motor_get_counts(motors -> left_motor_port);

    int right_judge = 0;
    int left_judge = 0;


    while (right_judge == 0 || left_judge == 0)
    {
        
        ev3_motor_set_power(motors -> right_motor_port, right_power * (motors -> right_fix_sign));
        ev3_motor_set_power(motors -> left_motor_port, left_power * (motors -> left_fix_sign));
        
        if (0 < right_power * motors -> right_motor_fix)
        {
            if (fst_right + degree < ev3_motor_get_counts(motors -> right_motor_port))
            { right_judge = 1; }
        }
        else
        {
            if (fst_right - degree > ev3_motor_get_counts(motors -> right_motor_port))
            { right_judge = 1; }
        }

        if (0 < left_power * motors -> left_motor_fix)
        {
            if (fst_left + degree < ev3_motor_get_counts(motors -> left_motor_port))
            { left_judge = 1; }
        }
        else
        {
            if (fst_left - degree > ev3_motor_get_counts(motors -> left_motor_port))
            { left_judge = 1; }
        }

    }

    if (stop == 1)
    {
        ev3_motor_set_power(motors -> right_motor_port, 0);
        ev3_motor_set_power(motors -> left_motor_port, 0);
    }
    
}




void mv__motors_roe(ports_t *motors, degree angle, int power, micro_sec_t acc_tm, int acc_num, int stop)
{
    
    int move_to_dir_right;
    int move_to_dir_left;
    int angle_sign = (angle >= 0 ? 1 : -1);

    int right_forward = (int)((motors -> right_motor_fix) * (float)angle_sign * (float)power * -1.0);//ここの-1は回転するとき反対は逆回転になるため
    int left_forward = (int)((motors -> left_motor_fix) * (float)angle_sign * (float)power); 

    int right_sign = (right_forward >= 0 ? 1 : -1);
    int left_sign = (left_forward >= 0 ? 1 : -1);

    int aim = abs((int)((motors -> tire_distance * (double)angle) / (motors -> tire_diameter)));
    //int aim = abs(((motors -> tire_distance) * angle) / (motors -> tire_diameter));

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

    mv__motors_start_sin(motors,(angle_sign * power * -1),(angle_sign * power),acc_tm,acc_num);

    uint16_t right_loop = 0;
    uint16_t left_loop = 0;
    int now_right_counts;
    int now_left_counts;
    if (stop == 1)
    {

    
        float sin_start = 0.7;//sin関数で減速するタイミング(0 ~ 1)
        double sin_rad;

       

        int now_aim;


        while ((right_loop == 0) || (left_loop == 0))
        {
            now_right_counts = ev3_motor_get_counts(motors -> right_motor_port);
            now_left_counts = ev3_motor_get_counts(motors -> left_motor_port);

            now_aim = aim - (int)((float)(abs(move_to_dir_right - now_right_counts) + abs(move_to_dir_left - now_left_counts)) / 2.00);

            if ((int)(aim * sin_start) <= now_aim)
            {
                sin_rad = cos(((double)(abs((double)now_aim - ((double)aim * sin_start))) / ((double)aim * (1.00 - sin_start))) * (3.1415 / 2.0));
                right_forward = (int)((motors -> right_motor_fix) * (float)angle_sign * (float)power * -1.0 * (float)sin_rad);
                left_forward = (int)((motors -> left_motor_fix) * (float)angle_sign * (float)power * (float)sin_rad);

                //sprintf(a,"%f,%f__",fabs(right_forward),fabs(left_forward));
                //ev3_lcd_draw_string(a,0,16);
            }

            ev3_motor_set_power((motors -> right_motor_port), right_forward);
            ev3_motor_set_power((motors -> left_motor_port), left_forward);

            if (right_sign >= 0 && move_to_dir_right <= now_right_counts)
            {right_loop = 1;}
            else if (right_sign < 0 && move_to_dir_right >= now_right_counts)
            {right_loop = 1;}

            if (left_sign >= 0 && move_to_dir_left <= now_left_counts)
            {left_loop = 1;}
            else if (left_sign < 0 && move_to_dir_left >= now_left_counts)
            {left_loop = 1;}

            if (abs(right_forward) < 3)
            {
                right_loop = 1;
            }
            if (abs(left_forward) < 3)
            {
                left_loop = 1;
            }

        }
        ev3_motor_set_power(motors -> right_motor_port,0);
        ev3_motor_set_power(motors -> left_motor_port,0);
    }
    else
    {

        while ((right_loop == 0) || (left_loop == 0))
        {

            now_right_counts = ev3_motor_get_counts(motors -> right_motor_port);
            now_left_counts = ev3_motor_get_counts(motors -> left_motor_port);
            
            ev3_motor_set_power((motors -> right_motor_port), right_forward);
            ev3_motor_set_power((motors -> left_motor_port), left_forward);

            if (right_sign >= 0 && move_to_dir_right <= now_right_counts)
            {right_loop = 1;}
            else if (right_sign < 0 && move_to_dir_right >= now_right_counts)
            {right_loop = 1;}

            if (left_sign >= 0 && move_to_dir_left <= now_left_counts)
            {left_loop = 1;}
            else if (left_sign < 0 && move_to_dir_left >= now_left_counts)
            {left_loop = 1;}
        }
        ev3_motor_set_power(motors -> right_motor_port,0);
        ev3_motor_set_power(motors -> left_motor_port,0);
        
    }
    

}



int dist_to_dir(ports_t *ports, float distance)
{
    return (int)((float)(360 * distance) / (ports -> tire_diameter * 3.14159));
}

#endif
