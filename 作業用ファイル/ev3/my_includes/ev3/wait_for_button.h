#include "ev3api.h"
#include "app.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>

#ifndef _H_WAIT_FOR_BUTTON_
#define _H_WAIT_FOR_BUTTON_

void wait_for_button(button_t button)
{

    ev3_led_set_color(LED_RED);
    while (ev3_button_is_pressed(button) == false)
    {

    }
    while (!(ev3_button_is_pressed(button) == false))
    {

    }
    ev3_led_set_color(LED_GREEN);
    
}

#endif