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
    NORMAL_START = 0,
    SIN_START = 1,
} start_situation_t;

typedef enum {
    NORMAL_END = 0,
    SIN_END = 1,
} end_situation_t;


//関数のプロトタイプ宣言
void mv__motor_start(ports_t *motors ,int right_power, int left_power);
void mv__motor_stop(ports_t *motors);
void mv__motor_start_sin(ports_t *motors, int right_power, int left_power, micro_sec_t wait, int time);
void mv__motor_stop_sin(ports_t *motors, micro_sec_t waittime, int times);
void my__motor_start_pro(ports_t *motors, int right_power, int left_power, micro_sec_t wait, int time);
void my__motor_stop_pro(ports_t *motors, micro_sec_t wait, int time);


//モーター開始関数
void mv__motor_start(ports_t *motors ,int right_power, int left_power)
{
    ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_motor_fix);
    ev3_motor_set_power(motors -> left_motor_port, left_power * motors -> left_motor_fix);
}

//モーター停止関数
void mv__motor_stop(ports_t *motors)
{
    ev3_motor_set_power(motors -> right_motor_port,0);
    ev3_motor_set_power(motors -> left_motor_port,0);
}

//モーター三角関数開始
void mv__motor_start_sin(ports_t *motors, int right_power, int left_power, micro_sec_t wait, int time)
{

    ev3_motor_set_power(motors -> right_motor_port,0);
    ev3_motor_set_power(motors ->  left_motor_port,0);

    double radian;
    for (int i = 0; i < time; i++)
    {

        radian = sin((3.1415 / 2.0) * (((double)i + 1.0000) / (double)time));
        ev3_motor_set_power((motors -> right_motor_port),(int)(((double)right_power * radian) * (double)(motors -> right_motor_fix)));
        ev3_motor_set_power((motors -> left_motor_port),(int)(((double)left_power * radian) * (double)(motors -> left_motor_fix)));

        tslp_tsk(wait);
    }

    ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_fix_sign);
    ev3_motor_set_power(motors -> left_motor_port,left_power * motors -> left_fix_sign);

}

//モーター三角関数停止関数
void mv__motor_stop_sin(ports_t *motors, micro_sec_t waittime, int times)
{
    int fst_right = ev3_motor_get_power(motors -> right_motor_port);
    int fst_left = ev3_motor_get_power(motors -> left_motor_port);
    float sin_val;

    for (int i = 0; i < times; i++)
    {
        sin_val = sin((3.1415 / 2) * ((float)(times - i) / times));
        ev3_motor_set_power(motors -> right_motor_port,((int)(fst_right * sin_val)));
        ev3_motor_set_power(motors -> left_motor_port,((int)(fst_left * sin_val)));

        tslp_tsk(waittime);
    }

    mv__motor_stop(motors);
    
}

void mv__motor_start_pro(ports_t *motors,int right_power,int left_power,micro_sec_t wait, int time)
{
    float pro = 0;
    for (int i = 1;i < time + 1;i++)
    {
        pro = (float)i / time;
        ev3_motor_set_power(motors -> right_motor_port,right_power * pro);
        ev3_motor_set_power(motors -> left_motor_port,left_power * pro);
    }
    ev3_motor_set_power(motors -> right_motor_port,right_power);
    ev3_motor_set_power(motors -> left_motor_port,left_power);
}

//モーター秒数間
void mv__motor_run_time(ports_t *motors,int right_power, int left_power, micro_sec_t time,start_situation_t start,end_situation_t end)
{

    if (start)
    {
        mv__motor_start_sin(motors,right_power,left_power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else
    {
        ev3_motor_set_power(motors -> right_motor_port,right_power * motors -> right_fix_sign);
        ev3_motor_set_power(motors -> left_motor_port,left_power * motors -> left_fix_sign);
    }

    tslp_tsk(time);

    if (end)
    {
        mv__motor_stop_sin(motors,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else
    {
        mv__motor_stop(motors);
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
    if (start)
    {
        mv__motor_start_sin(motors,power,power,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else
    {
        mv__motor_start(motors,power,power);
    }
    int ave;
    if (stop)
    {
        ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        while(ave < goal * _START_SIN_)
        {
            mv__motor_start(motors,power,power);
            ave = (abs(ev3_motor_get_counts(motors -> right_motor_port) - first_right) + abs(ev3_motor_get_counts(motors -> left_motor_port) - first_left)) / 2;
        }
        while (ave < goal)
        {
            float s = sin((ave - goal * _START_SIN_) / (goal * (1.0 - _START_SIN_)) * (_MY_PI_ / 2)) + _GAIN_;
            if (1.0 < s)
            {
                s = 1.0;
            }
            mv__motor_start(motors,(int)(power * s),(int)(power * s));
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



//モーターcm待機
void mv__motor_cm_wait(ports_t *port,uint16_t set_fst,int fst, motor_port_t motor, float cm)
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








void mv__motor_run_rotate(ports_t *motors, int right_power, int left_power, double degree, int stop)
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

void mv__motor_jiku_rotate(ports_t *ports, int direction, int degree, int power,end_situation_t end)
{

    int aim = (2 * (ports -> tire_distance) * abs(degree)) / (ports -> tire_diameter);

    int judge;
    int fst;
    if (0 < degree)
    {
        judge = 1;
    }
    else
    {
        judge = -1;
    }
    

    if (0 < direction)
    {
        fst = ev3_motor_get_counts(ports -> left_motor_port);
        ev3_motor_set_power(ports -> right_motor_port, 0);
        ev3_motor_set_power(ports -> left_motor_port, power * judge * ports -> left_motor_fix);
        if (0 < judge * ports -> left_motor_fix)
        {
            while (ev3_motor_get_counts(ports -> left_motor_port) < fst + aim)
            {
                ev3_motor_set_power(ports -> left_motor_port, power * judge * ports -> left_motor_fix);
            }
        }
        else
        {
            while (ev3_motor_get_counts(ports -> left_motor_port) > fst - aim)
            {
                ev3_motor_set_power(ports -> left_motor_port, power * judge * ports -> left_motor_fix);
            }
        }
        
    }
    else
    {
        fst = ev3_motor_get_counts(ports -> right_motor_port);
        ev3_motor_set_power(ports -> left_motor_port, 0);
        ev3_motor_set_power(ports -> right_motor_port, power * judge * ports -> right_motor_fix);
        if (0 < judge * ports -> right_motor_fix)
        {
            while (ev3_motor_get_counts(ports -> right_motor_port) < fst + aim)
            {
                ev3_motor_set_power(ports -> right_motor_port, power * judge * ports -> right_motor_fix);
            }
        }
        else
        {
            while (ev3_motor_get_counts(ports -> right_motor_port) > fst - aim)
            {
                ev3_motor_set_power(ports -> right_motor_port, power * judge * ports -> right_motor_fix);
            }
        }
    }
    if (end)
    {
        mv__motor_stop_sin(ports,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else
    {
       ev3_motor_set_power(ports -> right_motor_port,0);
        ev3_motor_set_power(ports -> left_motor_port,0); 
    }
    
    
}


void mv__motor_rotate(ports_t *motors, degree angle, int power, micro_sec_t acc_tm, int acc_num, int stop)
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

    mv__motor_start_sin(motors,(angle_sign * power * -1),(angle_sign * power),acc_tm,acc_num);

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
