#include <stdio.h>
#include "ev3api.h"
#include <stdlib.h>
#include "math.h"

#ifndef _H_SORT_
#define _H_SORT_

#define _PI_ 3.1415926
#define _INITIAL_SIZE_ 7

typedef struct {

    //モーターのポート＆修正値
    motor_port_t right_motor_port;
    motor_port_t left_motor_port;
    float right_motor_fix;
    float left_motor_fix;
    
    //カラーセンサーのポート
    sensor_port_t left_sensor_port;
    sensor_port_t right_sensor_port;

    //それぞれのモーターパワー -> 削除

    //機体のタイヤ距離＆タイヤ直径
    float tire_diameter;
    float tire_distance; 

    //変換
    float dir_convert_val;
    float dis_convert_val;

    //修正値の符号 
    uint8_t right_fix_sign;
    uint8_t left_fix_sign;

    //ライントレース初回検知
    int linetrase_fst;

    int steering_row;
    float steering;
    float past_ster;

    int *past_reflection;
    uint16_t writing_pos;
    uint16_t loading_len;
    uint16_t len;
    int duration;
    float ave;

    float log;

} ports_t;

void sorts_set_motors(ports_t *sort, motor_port_t right_port, motor_port_t left_port, float right_fix, float left_fix)
{

    sort -> right_motor_port = right_port;
    sort -> left_motor_port = left_port;
    sort -> right_motor_fix = right_fix;
    sort -> left_motor_fix = left_fix;

    sort -> right_fix_sign = right_fix / abs(right_fix);
    sort -> left_fix_sign = left_fix / abs(left_fix);

    sort -> log = 0;

}

void sorts_set_tire(ports_t *sort, double diameter, double distance)
{

    sort -> tire_diameter = diameter;
    sort -> tire_distance = distance;
    sort -> dis_convert_val = 360.0 / (diameter * _PI_); //距離→角度
    sort -> dir_convert_val = 1.0 / sort -> dis_convert_val; //角度→距離

    ev3_motor_reset_counts(sort -> right_motor_port);
    ev3_motor_reset_counts(sort -> left_motor_port);

}

void sorts_set_colors(ports_t *sort, sensor_port_t right_color, sensor_port_t left_color)
{

    sort -> right_sensor_port = right_color;
    sort -> left_sensor_port = left_color;

    sort -> linetrase_fst = 1;
    
}

//ライントレース初期化、ライントレースを始める際毎回実行
void sorts_linetrase_reset(ports_t *sort)
{
    free((void *)sort -> past_reflection);
    sort -> linetrase_fst = 1;
    sort -> loading_len = 1;
    sort -> writing_pos = 0;
    sort -> duration = 0;
    sort -> ave = 0;
}

void sorts_linetrase_for_init(ports_t *sort,int len)
{
    sort -> len = len;
    sort -> past_reflection = (int *)malloc(len * sizeof(int));
    sort -> loading_len = 1; //長さなので初期値は１
    sort -> writing_pos = 0;
}

void sorts_linetrase_for_change_len(ports_t *sort,int len)
{
    sort -> len = len;
    sort -> past_reflection = (int *)realloc(sort -> past_reflection,len * sizeof(int));
    sort -> loading_len = 1;
    sort -> writing_pos = 0;
}

float sorts_linetrase_for_average(ports_t *sort)
{
    int sum = 0;
    for (int i = 0; i < sort -> loading_len; i++)
    {
        sum += abs(sort -> past_reflection[i]);
    }
    return ((float)sum / sort -> loading_len);
}


void sorts_linetrase_for_write(ports_t *sort, int val)
{
    //代入
    sort -> past_reflection[sort -> writing_pos] = val;
    //インデックス更新
    if (sort -> len - 1 <= sort -> writing_pos)
    {
        sort -> writing_pos = 0;
    }
    else
    {
        sort -> writing_pos++;
    }
    //書き込みおわった長さ確認
    if (sort -> loading_len != sort -> len)
    {
        sort -> loading_len += 1;
    }
    
}
#endif