#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifndef _H_RANDOM_
#define _H_RANDOM_

int random_between(int min_val, int max_val)
{
    float result = (float)min_val + ((float)(max_val - min_val) * ((float)rand() / 2147483647.0));
    return (int)result;
}

#endif
