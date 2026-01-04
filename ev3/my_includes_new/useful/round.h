#include "ev3api.h"
#include "app.h"
#include <stdio.h>
#include "math.h"


int round_off_double(double a)
{

    return((floor(a + 0.5))(int));

}

int round_off_float(float a)
{

    return((floor(a + 0.5))(int));

}