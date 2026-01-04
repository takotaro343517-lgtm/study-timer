#include <stdio.h>

#ifndef _H_MY_AVERAGE_
#define _H_MY_AVERAGE_

float average(int *array, int size)
{
    int sum = 0;
    for (int *i = array; i < size;i++)
    {
        sum += *i;
    }
    return ((float)sum / size);
}

#endif