#include "ev3api.h"
#include <stdio.h>
#include "math.h"

#ifndef _H_BUTTON_
#define _H_BUTTON_

typedef struct {

    unsigned int up_judge;
    unsigned int down_judge;
    unsigned int left_judge;
    unsigned int right_judge;
    unsigned int enter_judge;

} buttons_t;

void key_detection_init(buttons_t *button_p)
{
    button_p -> enter_judge = 0;
    button_p -> up_judge = 0;
    button_p -> down_judge = 0;
    button_p -> left_judge = 0;
    button_p -> right_judge = 0;
}

void key_detection(buttons_t *button_p)//detectionは必ずループ文の一番最後に書くこと
{

    if (!(ev3_button_is_pressed(ENTER_BUTTON)))
    {
        button_p -> enter_judge = 0;
    }
    if (!(ev3_button_is_pressed(UP_BUTTON)))
    {
        button_p -> up_judge = 0;
    }
    if (!(ev3_button_is_pressed(DOWN_BUTTON)))
    {
        button_p -> down_judge = 0;
    }
    if (!(ev3_button_is_pressed(RIGHT_BUTTON)))
    {
        button_p -> right_judge = 0;
    }
    if (!(ev3_button_is_pressed(LEFT_BUTTON)))
    {
        button_p -> left_judge = 0;
    }

}

bool_t key_touch_start(buttons_t *button_p, button_t button)//真中１上２右３下４左５
{

    switch (button)
    {
    
        case UP_BUTTON :

            if ((ev3_button_is_pressed(UP_BUTTON)) && (button_p -> up_judge == 0))
            {
                button_p -> up_judge = 1;
                return true;
            }
            else
            {
                return false;
            }


        case DOWN_BUTTON :
        
            if ((ev3_button_is_pressed(DOWN_BUTTON)) && (button_p -> down_judge == 0))
            {
                button_p -> down_judge = 1;
                return true;
            }
            else
            {
                return false;
            }

        
        
        case ENTER_BUTTON :
        
            if ((ev3_button_is_pressed(ENTER_BUTTON)) && (button_p -> enter_judge == 0))
            {
                button_p -> enter_judge = 1;
                return true;
            }
            else
            {
                return false;
            }
        
        
        case RIGHT_BUTTON :
        
            if ((ev3_button_is_pressed(RIGHT_BUTTON)) && (button_p -> right_judge == 0))
            {
                button_p -> right_judge = 1;
                return true;
            }
            else
            {
                return false;
            }
        
        
        case LEFT_BUTTON :
        
            if ((ev3_button_is_pressed(LEFT_BUTTON)) && (button_p -> left_judge == 0))
            {
                button_p -> left_judge = 1;
                return true;
            }
            else
            {
                return false;
            }

        default :
        
            return false;
        
    }
}

int key_touch_finish(buttons_t *button_p, button_t button)
{

    if (button == ENTER_BUTTON)
    {
        if (ev3_button_is_pressed(button) == false && (button_p -> enter_judge == 1))
        {
            return (1);
        }
        else
        {
            return (0);
        }
    }
    else if (button == UP_BUTTON)
    {
        if (ev3_button_is_pressed(button) == false && (button_p -> up_judge == 1))
        {
            return (1);
        }
        else
        {
            return (0);
        }
    }
    else if (button == DOWN_BUTTON)
    {
        if (ev3_button_is_pressed(button) == false && (button_p -> down_judge == 1))
        {
            return (1);
        }
        else
        {
            return (0);
        }
    }
    else if (button == RIGHT_BUTTON)
    {
        if (ev3_button_is_pressed(button) == false && (button_p -> right_judge == 1))
        {
            return (1);
        }
        else
        {
            return (0);
        }
    }
    else if (button == LEFT_BUTTON)
    {
        if (ev3_button_is_pressed(button) == false && (button_p -> left_judge == 1))
        {
            return (1);
        }
        else
        {
            return (0);
        }
    }
}

#endif