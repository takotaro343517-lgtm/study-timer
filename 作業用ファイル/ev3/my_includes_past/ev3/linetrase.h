#include "ev3api.h"
#include "app.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
#include "sorts.h"
#include "steering.h"
#include "motor_movement_ster.h"

#ifndef _H_LINETRASE_
#define _H_LINETRASE_

void linetrase_pd(ports_t *linetrase, int speed, double p_gain,double d_gain);

void linetrase_pd(ports_t *linetrase, int speed, double p_gain,double d_gain)
{

    int right_color = ev3_color_sensor_get_reflect(linetrase -> right_sensor_port);
    int left_color = ev3_color_sensor_get_reflect(linetrase -> left_sensor_port);
    
    int steering = (left_color - right_color);
    
    //ライントレース初回の時の処理
    if ((linetrase -> linetrase_fst) == 1)
    {
        linetrase -> past_ster = steering;
        linetrase -> linetrase_fst = 0;
    }
    float final_steering = (p_gain * (steering)) + (d_gain * (steering - linetrase -> past_ster));

    steering_start(linetrase, speed, final_steering);
    
    linetrase -> past_ster = steering;

    linetrase -> steering_row = (left_color - right_color);
    linetrase -> steering = final_steering;

}


void linetrase_pd_cm(ports_t *linetrase,int speed,double p_gain,double d_gain,float cm,end_situation_t end)
{
    int past_r = ev3_motor_get_counts(linetrase -> right_motor_port);
    int past_l = ev3_motor_get_counts(linetrase -> left_motor_port);

    sorts_linetrase_reset(linetrase);
    while((abs(ev3_motor_get_counts(linetrase -> right_motor_port) - past_r) + abs(ev3_motor_get_counts(linetrase -> left_motor_port) - past_l)) / 2 * linetrase -> dir_convert_val < cm)
    {
        linetrase_pd(linetrase,speed,p_gain,d_gain);
    }
    if (end == NORMAL_END)
    {
        mv__motors_start(linetrase,0,0);
    }
    else if (end == SIN_END)
    {
        mv__motors_stop_sin(linetrase,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (end == PRO_END)
    {
        mv__motors_stop_pro(linetrase,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (end == NONE_END)
    {
        
    }
}

#define _H_FOR_RIGHT_ 1
#define _H_FOR_LEFT_ 2
#define _H_FOR_DOUBLE_ 3

#define _H_STER_LIMIT_ 15

uint8_t linetrase_pd_for(ports_t *linetrase, int speed, double p_gain,double d_gain,int to,int right_black,int left_black)
{
    if (linetrase -> linetrase_fst)
    {
        linetrase -> past_reflection = (int *)realloc(linetrase -> past_reflection,linetrase -> len * sizeof(int));//初回にメモリ確保
    }
    

    uint8_t finish = 0;

    int right_color = ev3_color_sensor_get_reflect(linetrase -> right_sensor_port);
    int left_color = ev3_color_sensor_get_reflect(linetrase -> left_sensor_port);
    
    float steering = left_color - right_color;

    //ライントレース初回の時過去のステアリングを今と同じままで初期値とする
    linetrase -> past_ster = (linetrase -> linetrase_fst == 1) ? steering : linetrase -> past_ster;

    float final_steering = (p_gain * (steering)) + (d_gain * (steering - linetrase -> past_ster));

    steering_start(linetrase, speed, final_steering);
    //動作開始

    linetrase -> past_ster = steering;
    //過去書き込み完了
    
    //過去配列に格納、インデックス更新
    sorts_linetrase_for_write(linetrase,(left_color - right_color));

    linetrase -> ave = sorts_linetrase_for_average(linetrase);


    //直角検知
    if (linetrase -> loading_len == linetrase -> len)
    {
        if (to == _H_FOR_RIGHT_ && right_color < right_black && abs((int)linetrase -> ave) < _H_STER_LIMIT_)
        {
            finish = 1;
        }
        else if (to == _H_FOR_LEFT_ && left_color < left_black && abs((int)linetrase -> ave) < _H_STER_LIMIT_)
        {
            finish = 1;
        }
        else if (to == _H_FOR_DOUBLE_ && left_color < left_black && right_color < right_black && abs((int)linetrase -> ave) < _H_STER_LIMIT_)
        {
            finish = 1;
        }
    }

    //継続時間更新
    linetrase -> duration++;

    //値書き込み
    linetrase -> steering_row = (left_color - right_color);
    linetrase -> steering = final_steering;

    //初回検知処理
    if (linetrase -> linetrase_fst == 1)
    {
        linetrase -> linetrase_fst = 0;
    }

    if (finish == 1)
    {
        free((void *)linetrase -> past_reflection);
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t linetrase_pd_for_new(ports_t *linetrase, int speed, double p_gain,double d_gain,int to,int right_black,int left_black,end_situation_t end)
{
    int finish = 0;
    sorts_linetrase_reset(linetrase);
    while(finish == 0)
    {
        finish = linetrase_pd_for(linetrase,speed,p_gain,d_gain,to,right_black,left_black);
    }
    if (end == NORMAL_END)
    {
        mv__motors_stop(linetrase);
    }
    else if (end == SIN_END)
    {
        mv__motors_stop_sin(linetrase,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (end == PRO_END)
    {
        mv__motors_stop_pro(linetrase,_DEFAULT_SIN_WAIT_,_DEFAULT_SIN_TIME_);
    }
    else if (end == NONE_END)
    {
        
    }
}



#endif