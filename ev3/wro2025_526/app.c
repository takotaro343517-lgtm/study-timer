#include "ev3api.h"
#include "app.h"
#include <stdio.h>

//インクルード
#include "my_includes/ev3/sorts.h"
#include "my_includes/ev3/motor_movement_ster.h"
#include "my_includes/ev3/linetrase.h"
#include "my_includes/useful/rgb_to_hsv.h"
#include "my_includes/ev3/wait_for_button.h"
#include "my_includes/ev3/one_linetrase.h"

//ライントレース適正地
//speed 40 20
//p 0.6 0.8
//d 6.0 4.0
//
#define Right_motor EV3_PORT_D
#define Left_motor EV3_PORT_A
#define Arm_motor EV3_PORT_B
#define Left_color EV3_PORT_2
#define Right_color EV3_PORT_3

#define MOVE_SPEED 35

#define LINE_SLOW_SPEED 20
#define LINE_SLOW_PGAIN 0.8
#define LINE_SLOW_DGAIN 4.0
#define LINE_SPEED 40
#define P_GAIN 0.2
#define D_GAIN 4.0
#define ONE_LINE_SPEED 20
#define ONE_P_GAIN 0.9
#define ONE_D_GAIN 5.5

#define RIGHT_WHITE 70
#define RIGHT_BLACK 21
#define LEFT_BLACK 11
#define LEFT_WHITE 65

#define CORNER 80

#define WAIT tslp_tsk(1000 * 500);

void to_black(ports_t *port,end_situation_t end)
{
    while((ev3_color_sensor_get_reflect(Right_color) > RIGHT_BLACK) && (ev3_color_sensor_get_reflect(Left_color) > LEFT_BLACK))
    {
        mv__motors_start(port,MOVE_SPEED,MOVE_SPEED);
    }
    if (end == NORMAL_END)
    {
        mv__motors_stop(port);
    }
    else if (end == NONE_END)
    {
        
    }
    
}

void to_white(ports_t *port,end_situation_t end)
{
    while((ev3_color_sensor_get_reflect(Right_color) < RIGHT_WHITE) && (ev3_color_sensor_get_reflect(Left_color) > LEFT_WHITE))
    {
        mv__motors_start(port,MOVE_SPEED,MOVE_SPEED);
    }
    if (end == NORMAL_END)
    {
        mv__motors_stop(port);
    }
    else if (end == NONE_END)
    {
        
    }
    
}

void arm_time(int power, micro_sec_t time)
{
    ev3_motor_set_power(Arm_motor,power);
    tslp_tsk(time);
    ev3_motor_set_power(Arm_motor,0);
}

void main_task(intptr_t unused)
{
    //ソート設定
    ev3_motor_config(Right_motor,MEDIUM_MOTOR);
    ev3_motor_config(Left_motor,MEDIUM_MOTOR);
    ev3_motor_config(Arm_motor,MEDIUM_MOTOR);
    ev3_sensor_config(Right_color,COLOR_SENSOR);
    ev3_sensor_config(Left_color,COLOR_SENSOR);

    ports_t _ports;
    ports_t *ports = &_ports;
    uint8_t finish = 0;
    uint16_t past_sensor;
    int past_right_motor;
    int past_left_motor;
    sorts_set_motors(ports,Right_motor,Left_motor,1,-1);
    sorts_set_colors(ports,Right_color,Left_color);
    sorts_set_tire(ports,6,17.5);

    sorts_linetrase_for_init(ports,50);

    wait_for_button(ENTER_BUTTON);

    
    mv__motors_run_cm(ports,MOVE_SPEED,20,PRO_START,PRO_END);
    WAIT

    linetrase_pd_cm(ports,LINE_SLOW_SPEED,LINE_SLOW_PGAIN,LINE_SLOW_DGAIN,8,NONE_END);
    linetrase_pd_cm(ports,LINE_SPEED,P_GAIN,D_GAIN,17,NORMAL_END);
    WAIT

    mv__motors_jiku_rotate(ports,-1,65,MOVE_SPEED,PRO_START,PRO_END);
    WAIT

    mv__motors_jiku_rotate(ports,1,65,MOVE_SPEED,PRO_START,PRO_END);
    WAIT

    to_black(ports,NONE_END);
    to_white(ports,NORMAL_END);
    WAIT

    mv__motors_start(ports,0,30);
    while(ev3_color_sensor_get_reflect(Left_color) > LEFT_BLACK)
    {
        
    }
    while(ev3_color_sensor_get_reflect(Left_color) < LEFT_WHITE)
    {
        
    }
    mv__motors_stop(ports);
    WAIT

    mv__motors_start(ports,30,-30);
    while(ev3_color_sensor_get_reflect(Left_color) > LEFT_BLACK)
    {
        
    }
    while(ev3_color_sensor_get_reflect(Left_color) <= LEFT_BLACK)
    {
        
    }
    mv__motors_stop(ports);
    WAIT
    
    linetrase_pd_cm(ports,LINE_SLOW_SPEED,LINE_SLOW_PGAIN,LINE_SLOW_DGAIN,12,NONE_END);
    linetrase_pd_cm(ports,LINE_SPEED,P_GAIN,D_GAIN,41,PRO_END);
    WAIT

    mv__motors_run_cm(ports,30,5,PRO_START,PRO_END);
    WAIT
    //アーム出とる
    arm_time(-30,1000 * 500);
    WAIT

    mv__motors_run_cm(ports,-30,23,PRO_START,PRO_END);
    WAIT
    
    mv__motors_start(ports,20,-20);
    while(ev3_color_sensor_get_reflect(Left_color) < LEFT_WHITE)
    {

    }
    while(ev3_color_sensor_get_reflect(Left_color) > LEFT_BLACK)
    {

    }
    mv__motors_stop(ports);
    WAIT

    linetrase_pd_cm(ports,LINE_SLOW_SPEED,LINE_SLOW_PGAIN,LINE_SLOW_DGAIN,10,NONE_END);
    linetrase_pd_cm(ports,LINE_SPEED,P_GAIN,D_GAIN,30,NORMAL_END);
    WAIT
    
    mv__motors_run_cm(ports,30,5,PRO_START,PRO_END);
    WAIT

    mv__motors_jiku_rotate(ports,1,45,MOVE_SPEED,PRO_START,PRO_END);
    WAIT
    
    mv__motors_run_cm(ports,MOVE_SPEED,22,PRO_START,PRO_END);
    WAIT
    //アーム戻す
    arm_time(30,1000 * 500);
    WAIT
    mv__motors_run_cm(ports,-MOVE_SPEED,22,PRO_START,PRO_END);
    WAIT

    mv__motors_jiku_rotate(ports,1,45,-MOVE_SPEED,PRO_START,PRO_END);
    WAIT

    mv__motors_rotate(ports,MOVE_SPEED,180,PRO_START,PRO_END);
    WAIT

    linetrase_pd_cm(ports,LINE_SLOW_SPEED,LINE_SLOW_PGAIN,LINE_SLOW_DGAIN,10,NONE_END);
    linetrase_pd_for_new(ports,LINE_SPEED,P_GAIN,D_GAIN,_H_FOR_DOUBLE_,RIGHT_BLACK,LEFT_BLACK,NONE_END);

    mv__motors_run_cm(ports,MOVE_SPEED,3,NORMAL_START,PRO_END);
    WAIT

    mv__motors_rotate(ports,30,90,PRO_START,PRO_END);
    WAIT

    linetrase_pd_cm(ports,LINE_SLOW_SPEED,LINE_SLOW_PGAIN,LINE_SLOW_DGAIN,10,PRO_END);
    WAIT

    mv__motors_run_cm(ports,LINE_SLOW_SPEED,17,PRO_START,PRO_END);
    WAIT

    //アームでとる
    arm_time(-30,1000 * 500);
    WAIT

    mv__motors_run_cm(ports,-MOVE_SPEED,30,PRO_START,PRO_END);
    WAIT
    mv__motors_start(ports,20,-20);
    while(ev3_color_sensor_get_reflect(Left_color) < LEFT_WHITE)
    {

    }
    while(ev3_color_sensor_get_reflect(Left_color) > LEFT_BLACK)
    {

    }
    mv__motors_stop(ports);
    
    WAIT

    linetrase_pd_cm(ports,LINE_SLOW_SPEED,LINE_SLOW_PGAIN,LINE_SLOW_DGAIN,10,NONE_END);
    linetrase_pd_cm(ports,LINE_SPEED,P_GAIN,D_GAIN,30,NORMAL_END);
    WAIT
    
    mv__motors_run_cm(ports,30,5,PRO_START,PRO_END);
    WAIT

    mv__motors_jiku_rotate(ports,1,130,MOVE_SPEED,PRO_START,PRO_END);
    WAIT

    mv__motors_run_cm(ports,MOVE_SPEED,5,PRO_START,PRO_END);
    WAIT
    //アーム戻す
    arm_time(30,1000 * 500);
    WAIT

    mv__motors_run_cm(ports,-MOVE_SPEED,8,PRO_START,PRO_END);
    WAIT

    mv__motors_jiku_rotate(ports,1,130,-MOVE_SPEED,PRO_START,PRO_END);
    WAIT

    mv__motors_rotate(ports,MOVE_SPEED,180,PRO_START,PRO_END);
    WAIT

    linetrase_pd_cm(ports,LINE_SLOW_SPEED,LINE_SLOW_PGAIN,LINE_SLOW_DGAIN,10,NONE_END);
    linetrase_pd_for_new(ports,LINE_SPEED,P_GAIN,D_GAIN,_H_FOR_DOUBLE_,RIGHT_BLACK,LEFT_BLACK,NONE_END);
    mv__motors_run_cm(ports,MOVE_SPEED,3,NORMAL_START,PRO_END);
    WAIT

    mv__motors_rotate(ports,MOVE_SPEED,-90,PRO_START,PRO_END);
    WAIT

    linetrase_pd_cm(ports,LINE_SLOW_SPEED,LINE_SLOW_PGAIN,LINE_SLOW_DGAIN,17,PRO_END);
    WAIT

    mv__motors_rotate(ports,-MOVE_SPEED,90,PRO_START,PRO_END);
    WAIT

    mv__motors_run_cm(ports,MOVE_SPEED,27,PRO_START,PRO_END);
    WAIT

    //引く
    arm_time(-30,1000 * 2000);
    WAIT

    mv__motors_run_cm(ports,-MOVE_SPEED,15,PRO_START,PRO_END);
    WAIT
    
    arm_time(30,1000 * 2000);
    WAIT

    mv__motors_run_cm(ports,-MOVE_SPEED,13,PRO_START,PRO_END);
    WAIT

    mv__motors_rotate(ports,MOVE_SPEED,90,PRO_START,PRO_END);
    WAIT

    linetrase_pd_cm(ports,LINE_SPEED,P_GAIN,D_GAIN,15,PRO_END);
    WAIT
    
    mv__motors_start(ports,-20,20);
    while(ev3_color_sensor_get_reflect(Left_color) < LEFT_WHITE)
    {

    }
    while(ev3_color_sensor_get_reflect(Left_color) > LEFT_BLACK)
    {

    }
    mv__motors_stop(ports);
    WAIT

    linetrase_pd_cm(ports,LINE_SLOW_SPEED,LINE_SLOW_PGAIN,LINE_SLOW_DGAIN,10,NONE_END);
    linetrase_pd_cm(ports,LINE_SPEED,P_GAIN,D_GAIN,30,PRO_END);
    WAIT

    mv__motors_jiku_rotate(ports,1,45,MOVE_SPEED,PRO_START,PRO_END);
    WAIT

    mv__motors_jiku_rotate(ports,-1,45,MOVE_SPEED,PRO_START,PRO_END);
    WAIT

    mv__motors_run_time(ports,MOVE_SPEED,MOVE_SPEED,1000 * 2000,NORMAL_START,PRO_END);
    WAIT

}
