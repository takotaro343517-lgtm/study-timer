#include "ev3api.h"
#include <stdio.h>
#include <stdint.h>

#ifndef_H_MY_ERROR_
#define _H_MY_ERROR_
void error_to_string(ER error,char *st)
{
    if (error = E_OK)
    {
        sprintf(st,"E_OK");
    }
    else if (error = E_ID)
    {
        sprintf(st,"E_ID");
    }
    else if (error = E_OBJ)
    {
        sprintf(st,"E_OBJ");
    }
}
#endif