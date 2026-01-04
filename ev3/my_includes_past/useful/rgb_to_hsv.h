#include <stdio.h>
#include "max_min.h"
#include "stdint.h"
#include "../../../sdk/common/ev3api/src/ev3api_sensor.h"


#ifndef _H_RGB_TO_HSV_
#define _H_RGB_TO_HSV_

//void rgb_to_hsv(const rgb_raw_t,hsv_raw_t);
//void ev3_color_sensor_get_hsv(sensor_port_t,hsv_raw_t);

typedef struct{

    uint16_t h;
    uint16_t s;
    uint16_t v;

}hsv_raw_t;

void rgb_to_hsv(const rgb_raw_t *rgb, hsv_raw_t *hsv)
{

    uint16_t r = rgb -> r;
    uint16_t g = rgb -> g;
    uint16_t b = rgb -> b;
    int rgb_list[3] = {r,g,b};

    uint16_t max = max_int(rgb_list,3);
    uint16_t min = min_int(rgb_list,3);

    int max_index = max_index_int(rgb_list,3);

    if (r == g && g == b)
    {
        hsv -> h = 0;
    }   
    else if (max_index == 0)
    {
        hsv -> h = 60 * ((r - g) / (max - min));
    }
    else if (max_index == 1)
    {
        hsv -> h = 60 * ((b - r) / (max - min)) + 120;
    }
    else
    {
        hsv -> h = 60 * ((b - r) / (max - min)) + 240;
    }

    hsv -> s =(max - min) / max;

    hsv -> v = max;

}

void ev3_color_sensor_get_hsv_raw(sensor_port_t port,hsv_raw_t *val)
{
    rgb_raw_t _rgb;
    rgb_raw_t *rgb = &_rgb;
    rgb_to_hsv(rgb,val);
}

#endif