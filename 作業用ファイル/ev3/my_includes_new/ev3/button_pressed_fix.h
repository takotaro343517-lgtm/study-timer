#include "ev3api.h"
#include <stdio.h>

#ifndef _H_MY_BUTTON_PRESSED_FIX_
#define _H_MY_BUTTON_PRESSED_FIX_

/*
ev3_button_is_pressed

なんかこのメソッドバグってる、falseはちゃんとfalseで戻り値が来るけどtrueのときがtrueではなくなぜか
2の31乗の負の値である-2,147,483,648　が出力される
*/

bool_t btf_button_fix_pressed(button_t button)
{

    if (ev3_button_is_pressed(button) == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}

#endif