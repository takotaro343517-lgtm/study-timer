#include <stdio.h>

#ifndef _H_MY_MAX_MIN_
#define _H_MY_MAX_MIN_

int max_int(const int from[], const int length)
{

    int max = from[0];

    for (int i=1; i < length; i++)
    {

        if (max < from[i])
        {
            max = from[i];
        }

    }

    return max;
}

int max_index_int(const int from[], const int length)
{

    int max = from[0];
    int max_index = 0;

    for (int i=1;i < length; i++)
    {

        if (max < from[i])
        {
            max = from[i];
            max_index = i;
        }

    }

    return max_index;
}

int min_int(const int from[], const int length)
{

    int min = from[0];

    for (int i=1;i < length; i++)
    {

        if (min > from[i])
        {
            min = from[i];
        }

    }

    return min;
}

int min_index_int(const int from[], const int length)
{

    int max = from[0];
    int max_index = 0;

    for (int i=1;i < length; i++)
    {

        if (max > from[i])
        {
            max = from[i];
            max_index = i;
        }

    }

    return max_index;
}

#endif

