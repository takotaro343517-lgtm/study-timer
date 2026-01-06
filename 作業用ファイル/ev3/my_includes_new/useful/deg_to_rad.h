#include <math.h>

#ifndef _H_MY_RADDEGREE_
#define _H_MY_RADDEGREE_

double sin_degree(double deg)
{

    double ans = sin(deg * (M_PI / 180));
    return ans;

}

#endif