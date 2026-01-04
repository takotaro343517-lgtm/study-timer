#include <stdio.h>

#ifndef _H_MY_CLAMP_
#define _H_MY_CLAMP_

int clamp_int(int val,int min,int max)
{
    if (val < min)
    {
        return min;
    }
    else if (max < val)
    {
        return max;
    }
    else 
    {
        return val;
    }
}

float clamp_float(float val,float min,float max)
{
    if (val < min)
    {
        return min;
    }
    else if (max < val)
    {
        return max;
    }
    else 
    {
        return val;
    }
}

#endif