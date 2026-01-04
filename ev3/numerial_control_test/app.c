#include "ev3api.h"
#include "app.h"
#include <stdio.h>
#include "my_includes_past/ev3/wait_for_button.h"
#include "my_includes_past/ev3/buttons.h"
#include "my_includes_past/ev3/motor_movement_ster.h"
#include "my_includes_past/useful/rgb_to_hsv.h"


#define Right_motor EV3_PORT_D
#define Left_motor EV3_PORT_A


static ports_t _ports;
static ports_t *ports = &_ports;
static int loop = 1;

void main_task(intptr_t unused)
{

    //コンフィグゾーン
    ev3_motor_config(Right_motor,MEDIUM_MOTOR);
    ev3_motor_config(Left_motor, MEDIUM_MOTOR);


    act_tsk(RUN_TASK);

}

void run_task(intptr_t unused)
{

    int speed = 40;
    float centimeter = 15.0;
    float p_gain = 0;
    float d_gain = 0;
    
    //dgain1.4just

    unsigned int menu = 1;
    unsigned int go = 0;
    char moji[150];
    //char *mojip = &moji;

    buttons_t _bt;
    buttons_t *bt = &_bt;

    sorts_set_motors(ports, Right_motor, Left_motor, 1,-1);
    sorts_set_tire(ports,6,17.5);

    wait_for_button(ENTER_BUTTON);

    key_detection(bt);

    while (1)
    {   

        if ((key_touch_start(bt,UP_BUTTON)) && (menu != 1))
        {
            menu -= 1;
        }
        else if ((key_touch_start(bt,DOWN_BUTTON)) && (menu != 4))
        {
            menu += 1;
        }
        else if (key_touch_start(bt,RIGHT_BUTTON))
        {
            if (menu == 1)
            {
                //speed += 10;
            }
            else if (menu == 2)
            {
                //centimeter += 1.0;
            }
            else if (menu == 3)
            {
                p_gain += 0.1;
            }
            else if (menu == 4)
            {
                d_gain += 0.1;
            }
        }
        else if (key_touch_start(bt,LEFT_BUTTON))
        {
            if (menu == 1)
            {
                //speed -= 10;
            }
            else if (menu == 2)
            {
                //centimeter -= 1.0;
            }
            else if (menu == 3)
            {
                p_gain -= 0.1;
            }
            else if (menu == 4)
            {
                d_gain -= 0.1;
            }
        }
        else if (key_touch_start(bt,ENTER_BUTTON))
        {
            if (go == 1)
            {
                go = 0;
            }
            else
            {
                go = 1;
            }
            
        }
        else
        {
            
        }
        
       
        if (go == 1)
        {
            mv__motors_run_cm_new(ports ,speed ,centimeter ,p_gain ,d_gain );
            
            go = 0;
        }
        else
        {
            mv__motors_stop(ports);
        }

        
        sprintf(moji,"pro : %f",pro);
        ev3_lcd_draw_string(moji,0,40);

        sprintf(moji,"pro : %f",pro);
        ev3_lcd_draw_string(moji,0,48);

        sprintf(moji,"menu : %d",menu);
        ev3_lcd_draw_string(moji,0,56);

        sprintf(moji,"value : ");
        ev3_lcd_draw_string(moji,0,64);
        
        sprintf(moji,"steering_row : %d___",ports -> steering_row);
        ev3_lcd_draw_string(moji,0,72);

        sprintf(moji,"steering : %f___",ports -> steering);
        ev3_lcd_draw_string(moji,0,80);

        sprintf(moji,"ave : %f___",ports -> ave);
        ev3_lcd_draw_string(moji,0,88);

        key_detection(bt);
        
        tslp_tsk(50);
    }
   
}
 