#include "ev3api.h"

#ifndef _H_RGB_FIX_
#define _H_RGB_FIX_

void ev3_color_sensor_get_rgb_fix(sensor_port_t port, rgb_raw_t *rgb)
{
    
    rgb_raw_t _rgb;
    rgb_raw_t *__rgb = &_rgb;
    ev3_color_sensor_get_rgb_raw(port,__rgb);

    if (_rgb.r > 255)
    {
        _rgb.r = 255;
    }

    if (_rgb.g > 255)
    {
        _rgb.g = 255;
    }

    if (_rgb.b > 255)
    {
        _rgb.b = 255;
    }

    rgb -> r = _rgb.r;
    rgb -> g = _rgb.g;
    rgb -> b = _rgb.b;

}

#endif