/*
 * helpers.c
 *
 *  Created on: 28 Apr 2023
 *      Author: Master
 */

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void saturated_add_3(signed long *result, signed long a, signed long b)
{
    if (a > 0 && b > 0)
    {
        Uint64 temp_res = (Uint32)a + (Uint32)b;
        if (temp_res > 4200000 - 1)
        {
            *result = 4200000;
        }
        else
        {
            signed long res = (signed long)a + (signed long)b;
            *result =  res;
        }
    }else
    {
        // Implement underflow protection
        *result = a + b;
    }
}

signed long saturate(signed long value, signed long min, signed long max)
{
    return MIN(MAX(value, min), max);
}


Uint32 map(signed long x, signed long in_min, signed long in_max, Uint32 out_min, Uint32 out_max)
{
    return ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}
